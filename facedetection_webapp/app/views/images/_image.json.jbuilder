json.extract! image, :id, :name, :width, :height, :x_position, :y_position, :created_at, :updated_at
json.url image_url(image, format: :json)
