# devops_workshop

This is a repository to support the AIT AI Center's DevOps workshop.

## Scenario

We'll use a toy application composed of a Ruby on Rails Web application, a C++ microservice, and a MySQL database.

The application grabs an image from the user's webcam, submits it to the microservice for face detection, then
inserts information about any detections into the database.

You could imagine a Web-scale system like Facebook using such a service to search uploaded photos for faces.

## Dev environment

First, let's run the software in the Web environment.

The development environment runs in a single Docker container. Make sure you have docker installed.
On Ubuntu, it should be enough to run

    $ sudo apt install docker

Once docker is installed, to get the dev environment up and running, do

    $ ./scripts/run-dev.sh

This should build the Docker image, start a Docker container with the image,
build our software in the container, and start the face detection service.

The service is a very simple REST service that accepts HTTP POST requests
to <pre>/images</pre>.

The POST data should be JSON-formatted like so:

    {
        "faceId": "123",
        "imageData": "/9j/..."
    }

The <pre>imageData</pre> element should be a JPEG encoding of an image, in turn encoded to a text string with base64.

There is a sample image <pre>test.jpg</pre> and a sample JSON file <pre>smoketest.json</pre> in the
<pre>scripts/</pre> directory.

To smoke test your service after it's up and running, you should be able to run the script

    $ ./scripts/smoketest-dev.sh

If all is well, you'll get a message indicating the service is up and running.

