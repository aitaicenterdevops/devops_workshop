Given("I am on the homepage") do
  visit root_path
  expect(page).to have_link("New Image", :href => new_image_path)
end

When("I click on link for new image") do
  click_on 'New Image'
end

Then("I should see form for new image") do
  expect(page).to redirect_to(new_image_path)
end

