class AddCroppedFaceInfoToFImage64s < ActiveRecord::Migration[5.1]
  def change
    add_column :fimage64s, :faceFound, :bool
    add_column :fimage64s, :faceWidth, :integer
    add_column :fimage64s, :faceHeight, :integer
  end
end
