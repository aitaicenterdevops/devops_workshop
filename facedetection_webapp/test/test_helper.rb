require 'simplecov'
require 'webmock/minitest'
SimpleCov.start 'rails' do
  add_filter "app/channels"
  add_filter "app/mailers"
  add_filter "app/jobs"
end
ENV['RAILS_ENV'] ||= 'test'
require_relative '../config/environment'
require 'rails/test_help'

class ActiveSupport::TestCase

  # Setup all fixtures in test/fixtures/*.yml for all tests in alphabetical order.
  fixtures :all

  # Add more helper methods to be used by all tests here...
end
