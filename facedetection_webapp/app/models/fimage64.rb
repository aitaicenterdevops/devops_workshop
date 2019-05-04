class Fimage64 < ApplicationRecord

mount_base64_uploader :image, ImageUploader

end
