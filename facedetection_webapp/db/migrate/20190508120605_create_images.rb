class CreateImages < ActiveRecord::Migration[5.2]
  def change
    create_table :images do |t|
      t.string :name
      t.integer :width
      t.integer :height
      t.integer :x_position
      t.integer :y_position

      t.timestamps
    end
  end
end
