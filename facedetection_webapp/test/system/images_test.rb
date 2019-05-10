require "application_system_test_case"

class ImagesTest < ApplicationSystemTestCase
  setup do
    @image = images(:one)
  end

  test "visiting the index" do
    visit images_url
    assert_selector "h1", text: "Images"
  end

  test "creating a Image" do
    visit images_url
    click_on "New Image"

#    fill_in "Height", with: @image.height
    fill_in "Name", with: @image.name
#    fill_in "Width", with: @image.width
#    fill_in "X position", with: @image.x_position
#    fill_in "Y position", with: @image.y_position
    click_on "Capture"
    click_on "Create Image"

    assert_text "Image was successfully created"
    click_on "Back"
  end


  test "destroying a Image" do
    visit images_url
    page.accept_confirm do
      click_on "Destroy", match: :first
    end

    assert_text "Image was successfully destroyed"
  end
end
