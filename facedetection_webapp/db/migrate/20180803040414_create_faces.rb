class CreateFaces < ActiveRecord::Migration[5.1]
  def change
    create_table :faces do |t|
      t.string :filename
      t.text :description
      t.string :age
      t.string :gender
      t.string :sentiment

      t.timestamps
    end
  end
end
