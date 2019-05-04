class ImagesController < ApplicationController
  def show
    image = Image.find(params[:id])
    render json: image, except: [:data]
  end

  def create
    image = Image.new
    image.tag = params[:tag]
    image.save!
    render json: image, except: [:data]
  end

  def download
    image = Image.find(params[:id])
    send_data image.data, type: "image/png", disposition: 'inline'
  end

  def upload
    image = Image.find(params[:id])
    image.data = request.raw_post
    image.save!
    render json: image, except: [:data]
  end
end
