require 'test_helper'

class ImageTest < ActiveSupport::TestCase


  test "should not save image without name" do

    test_image = Image.new
    assert_not test_image.save, "Saved image without name"

    test_image.update(name: "test_name")

    assert test_image.save, "Save successfully"


  end



end
