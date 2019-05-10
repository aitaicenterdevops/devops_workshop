Feature: See first page
  As a user
  I want to see homepage when visiting the site

Scenario: access homepage
  Given I am on the homepage
  When I click on link for new image
  Then I should see form for new image
