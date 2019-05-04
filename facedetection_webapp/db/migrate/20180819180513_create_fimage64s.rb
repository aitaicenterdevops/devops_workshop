class CreateFimage64s < ActiveRecord::Migration[5.1]
  def change
    create_table :fimage64s do |t|
      t.string :image
      t.string :tag
      t.string :name

      t.timestamps
    end
  end
end
