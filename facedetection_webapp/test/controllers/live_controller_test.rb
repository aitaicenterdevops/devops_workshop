require 'test_helper'

class LiveControllerTest < ActionDispatch::IntegrationTest
  test "should get new" do
    get live_new_url
    assert_response :success
  end

  test "should get create" do
    get live_create_url
    assert_response :success
  end

end
