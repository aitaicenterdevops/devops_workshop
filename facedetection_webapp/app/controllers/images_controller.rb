class ImagesController < ApplicationController
  before_action :set_image, only: [:show, :edit, :update, :destroy]

  # GET /images
  # GET /images.json
  def index
    @images = Image.all
  end

  # GET /images/1
  # GET /images/1.json
  def show
  end

  # GET /images/new
  def new
    @image = Image.new
  end

  def create

    @image = Image.new(image_params)

    respond_to do |format|
      if @image.save

        unless params[:base64img].blank?
          face_detection(params[:base64img])
        end

        format.html { redirect_to @image, notice: 'Face detection completed.' }
        format.json { render :show, status: :created, location: @image }
      else
        format.html { render :new }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end


  # DELETE /images/1
  # DELETE /images/1.json
  def destroy
    @image.destroy
    respond_to do |format|
      format.html { redirect_to images_url, notice: 'Image was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_image
      @image = Image.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def image_params
      params.require(:image).permit(:name, :width, :height, :x_position, :y_position, :original_image, :face_image)
    end

    def face_detection(taken_image)
      require 'net/http'
      require 'json'
      require 'uri'
      require 'mini_magick'


      base64img = taken_image.split('base64,')[1]
      @image.original_image.attach(data: taken_image, filename: 'original.png', content_type: 'image/png')

      url = URI.parse(ENV["FACE_DETECTOR_URL"].to_s)
      http = Net::HTTP.new(url.host,url.port)
      data = {faceId: @image.id , imageData: base64img}
      request = Net::HTTP::Post.new(url.request_uri,{'Content-Type' => 'application/json'})
      request.body = data.to_json
      response = http.request(request)

      puts "response from data analysis is #{response.body}"

      message = JSON.parse(response.body)['faces']

      puts "message is #{message.size}"

      if message.size > 0
        face_information = message.last
        puts face_information
        height = face_information["height"]
        width = face_information["width"]
        x_pos = face_information["x"]
        y_pos = face_information["y"]

        temp_file_name = Rails.root.join('tmp','storage', "temp_image_" + @image.id.to_s + ".png")

        imgData = Base64.decode64(taken_image.split('base64,')[1])

        File.open(temp_file_name, 'wb') do |f|
          f.write(imgData)
        end


        temp_im =MiniMagick::Image.open(temp_file_name)

        face_im = temp_im.crop "#{height.to_s}x#{width.to_s}+#{x_pos}+#{y_pos}"
        temp_face_file_name = Rails.root.join('tmp','storage', "temp_face_image_" + @image.id.to_s + ".png")
        face_im.write temp_face_file_name

        face_im_base64 =  "data:image/png;base64,"+Base64.encode64(File.open(temp_face_file_name, "rb").read)

        @image.face_image.attach(data: face_im_base64, filename: 'face.png', content_type: 'image/png')
        File.delete(temp_face_file_name) if File.exist?(temp_face_file_name)

        @image.update!(width: width.to_i, height: height.to_i, x_position: x_pos.to_i, y_position: y_pos.to_i)

      else
        puts "no faces information"
      end


    end

end
