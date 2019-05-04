require 'test_helper'

class FacesControllerTest < ActionDispatch::IntegrationTest
  setup do
    @face = faces(:one)
  end

  test "should get index" do
    get faces_url
    assert_response :success
  end

  test "should get new" do
    get new_face_url
    assert_response :success
  end

  test "should create face" do
    assert_difference('Face.count') do
      post faces_url, params: { face: { age: @face.age, description: @face.description, filename: @face.filename, gender: @face.gender, sentiment: @face.sentiment } }
    end

    assert_redirected_to face_url(Face.last)
  end

  test "should show face" do
    get face_url(@face)
    assert_response :success
  end

  test "should get edit" do
    get edit_face_url(@face)
    assert_response :success
  end

  test "should update face" do
    patch face_url(@face), params: { face: { age: @face.age, description: @face.description, filename: @face.filename, gender: @face.gender, sentiment: @face.sentiment } }
    assert_redirected_to face_url(@face)
  end

  test "should destroy face" do
    assert_difference('Face.count', -1) do
      delete face_url(@face)
    end

    assert_redirected_to faces_url
  end
end
