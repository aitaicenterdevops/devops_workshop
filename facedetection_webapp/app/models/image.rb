class Image < ApplicationRecord
  has_one_base64_attached :original_image
  has_one_base64_attached :face_image


end
