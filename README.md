
# DevOps Workshop Code Samples

This is a repository to support the AIT AI Center's DevOps workshop.

## Scenario

We'll use a toy application composed of a Ruby on Rails Web application, a C++ microservice, and a MySQL database.

The application grabs an image from the user's webcam, submits it to the microservice for face detection, then
inserts information about any detections into the database.

You could imagine a Web-scale system like Facebook using such a service to search uploaded photos for faces.

## Get started

First clone the repository and switch to the <pre>before-test</pre>
branch:

    $ git clone https://github.com/aitaicenterdevops/devops_workshop
    $ cd devops_workshop
    $ git checkout before-test

Now you see the initial version of our project, without regression tests
or deployment pipeline.

## Dev environment

First, let's try to run the software in the Web environment.

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

## Prototype build/test/deploy pipeline

Let's assume we've inherited this legacy project and it's our job to
get the code into a CI/CD pipeline with comprehensive
unit/integration/acceptance testing.

The first thing we should do is to get the project into a repeatable
pipeline that will trigger on each build.

As discussed in class, there are many tools for building CI/CD pipelines.
Jenkins is one of the well-known ones.  In real life, of course, you'd want
to run it in your data center, on a cloud server, or maybe you'd use the
Jenkins SaaS from CloudBees.  In this tutorial, we'll play with
Jenkins on the local host under Docker.  Let's pull a Docker image for
Jenkins:

    $ docker pull jenkins/jenkins:lts
    $ docker run -p 8080:8080 -p 50000:50000 -v jenkins_home:/var/jenkins_home jenkins/jenkins:lts

The volume option (<tt>-v</tt>) enables our local
Jenkins to save its state persistently rather than starting fresh from
scratch every time we start the container.  Later, if you need to
access any of that state, you can do

    docker ps

to get the container's ID then

    docker exec -it containerId bash

to run a shell in the container and poke around.

You can use a different host port instead of 8080 if you've already got
something like Tomcat running on port 8080.

OK, copy the initial setup password then visit
<a href="http://localhost:8080">http://localhost:8080</a> in a browser.
Use the given password, click on "Install suggested plugins" and wait
for the plugins to install. It'll probably take a few minutes.

While that's going on, let's create our first build pipeline.


There's a slightly more complete example in the
