require 'test_helper'

class Fimage64sControllerTest < ActionDispatch::IntegrationTest
  setup do
    @fimage64 = fimage64s(:one)
  end

  test "should get index" do
    get fimage64s_url
    assert_response :success
  end

  test "should get new" do
    get new_fimage64_url
    assert_response :success
  end

  test "should create fimage64" do
    assert_difference('Fimage64.count') do
      post fimage64s_url, params: { fimage64: { image: @fimage64.image, name: @fimage64.name, tag: @fimage64.tag } }
    end

    assert_redirected_to fimage64_url(Fimage64.last)
  end

  test "should show fimage64" do
    get fimage64_url(@fimage64)
    assert_response :success
  end

  test "should get edit" do
    get edit_fimage64_url(@fimage64)
    assert_response :success
  end

  test "should update fimage64" do
    patch fimage64_url(@fimage64), params: { fimage64: { image: @fimage64.image, name: @fimage64.name, tag: @fimage64.tag } }
    assert_redirected_to fimage64_url(@fimage64)
  end

  test "should destroy fimage64" do
    assert_difference('Fimage64.count', -1) do
      delete fimage64_url(@fimage64)
    end

    assert_redirected_to fimage64s_url
  end
end
