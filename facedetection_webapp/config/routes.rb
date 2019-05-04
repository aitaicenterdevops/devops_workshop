Rails.application.routes.draw do
  resources :fimage64s
  get 'live/new'

  get 'live/create'

  resources :faces
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html

  get 'images/:id' => 'images#show'
  post 'images' => 'images#create'

  get 'images/:id/download' => 'images#download'
  put 'images/:id/upload' => 'images#upload'

  post 'new2' => 'faces#new2'

end
