class FacesController < ApplicationController
  before_action :set_face, only: [:show, :edit, :update, :destroy]
  protect_from_forgery with: :null_session

  # GET /faces
  # GET /faces.json
  def index
    @faces = Face.all
  end

  # GET /faces/1
  # GET /faces/1.json
  def show
  end

  # GET /faces/new
  def new
    puts "Face Controller: new"
    @face = Face.new
  end

  def new2
	unless params[:hello].empty?
	  data =  params[:hello]
	  image_data = Base64.decode64(data['data:image/png;base64,'.length .. -1])
	  #File.open("#{Rails.root}/public#{@face.image.url.to_s}", 'wb') do |f|
      File.open("#{Rails.root}/public/tmp.png", 'wb') do |f|
		f.write image_data
		#params[:face][:filename] = "#{Rails.root}/public/tmp.png"
        #@face = Face.new(face_params)
	  end
	  #Carierwave method to regenerate thumbnails
	  #@face.image.recreate_versions!
	end
  end

  # GET /faces/1/edit
  def edit
  end

  # POST /faces
  # POST /faces.json
  def create
    puts "Face Controller: create"
    @face = Face.new(face_params)

    respond_to do |format|
      if @face.save
        format.html { redirect_to @face, notice: 'Face was successfully created.' }
        format.json { render :show, status: :created, location: @face }
      else
        format.html { render :new }
        format.json { render json: @face.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /faces/1
  # PATCH/PUT /faces/1.json
  def update
    respond_to do |format|
      if @face.update(face_params)
        format.html { redirect_to @face, notice: 'Face was successfully updated.' }
        format.json { render :show, status: :ok, location: @face }
      else
        format.html { render :edit }
        format.json { render json: @face.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /faces/1
  # DELETE /faces/1.json
  def destroy
    @face.destroy
    respond_to do |format|
      format.html { redirect_to faces_url, notice: 'Face was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_face
	  puts "Face Controller: set_face"
      @face = Face.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def face_params
      puts "Face Controller: face_params"
	  Rails.logger.info("PARAMS: #{params.inspect}")

      params.require(:face).permit(:filename, :description, :age, :gender, :sentiment)
    end
end
