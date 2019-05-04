class Fimage64sController < ApplicationController
  before_action :set_fimage64, only: [:show, :edit, :update, :destroy]
  protect_from_forgery with: :null_session

  # GET /fimage64s
  # GET /fimage64s.json
  def index
    @fimage64s = Fimage64.all
  end

  # GET /fimage64s/1
  # GET /fimage64s/1.json
  def show
  end

  # GET /fimage64s/new
  def new
    @fimage64 = Fimage64.new
  end

  # GET /fimage64s/1/edit
  def edit
  end

  # POST /fimage64s
  # POST /fimage64s.json
  def create
    @fimage64 = Fimage64.new(fimage64_params)
    puts 'image created'
	puts  @fimage64.image.to_s
 	

    respond_to do |format|
      if @fimage64.save
        format.html { redirect_to @fimage64, notice: 'Fimage64 was successfully created.' }
		puts  @fimage64.image.to_s
		@exeCommandStr = '/home/jednipat/works/devops_workshop/facedetection_va/facedetector' + " " + @fimage64.image.to_s
		system(@exeCommandStr)

        format.json { render :show, status: :created, location: @fimage64 }
      else
        format.html { render :new }
        format.json { render json: @fimage64.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /fimage64s/1
  # PATCH/PUT /fimage64s/1.json
  def update
    respond_to do |format|
      if @fimage64.update(fimage64_params)
        format.html { redirect_to @fimage64, notice: 'Fimage64 was successfully updated.' }
        format.json { render :show, status: :ok, location: @fimage64 }
      else
        format.html { render :edit }
        format.json { render json: @fimage64.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /fimage64s/1
  # DELETE /fimage64s/1.json
  def destroy
    @fimage64.destroy
    respond_to do |format|
      format.html { redirect_to fimage64s_url, notice: 'Fimage64 was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_fimage64
      @fimage64 = Fimage64.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def fimage64_params
      params.require(:fimage64).permit(:image, :tag, :name)
    end
end
