
# DevOps Workshop Code Samples

This is a repository to support the AIT AI Center's DevOps workshop.

## Scenario

We'll use a toy application composed of a Ruby on Rails Web application, a C++ microservice, and a MySQL database.

The application grabs an image from the user's webcam, submits it to the microservice for face detection, then
inserts information about any detections into the database.

You could imagine a Web-scale system like Facebook using such a service to search uploaded photos for faces.

## Get started

First clone the repository and switch to the <tt>before-test</tt>
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
to <tt>/images</tt>.

The POST data should be JSON-formatted like so:

    {
        "faceId": "123",
        "imageData": "/9j/..."
    }

The <tt>imageData</tt> element should be a JPEG encoding of an image, in turn encoded to a text string with base64.

There is a sample image <tt>test.jpg</tt> and a sample JSON file <tt>smoketest.json</tt> in the
<tt>scripts/</tt> directory.

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
Jenkins, then modify it for the purposes of today's tutorial.

    $ docker build -f dev/Dockerfile.jenkins dev/ -t jenkins-with-docker
    $ docker run -p 8080:8080 -p 50000:50000 \
                 -v jenkins_home:/var/jenkins_home \
                 -v  /var/run/docker.sock:/var/run/docker.sock \
                 -v /path/to/workdir/devops_workshop:/devops_workshop:ro \
                 --privileged \
                 jenkins-with-docker

The first volume option (<tt>-v</tt>) enables our local
Jenkins to save its state persistently rather than starting fresh from
scratch every time we start the container.  Later, if you need to
access any of that state, you can do

    docker ps

to get the container's ID then

    docker exec -it containerId bash

to run a shell in the container and poke around.

The second volume option is a "bind" mount of your local copy of of the
git repository to the path <tt>/devops\_workshop</tt> inside Jenkins'
Docker environment. In a bit, we'll have Jenkins monitor your repository
via this bind mount. The directory is mounted read only for safety.

You can use a different host port instead of 8080 if you've already got
something like Tomcat running on port 8080.

OK, copy the initial setup password then visit
<a href="http://localhost:8080">http://localhost:8080</a> in a browser.
Use the given password, click on "Install suggested plugins" and wait
for the plugins to install. It'll probably take a few minutes.

While that's going on, let's create our first build pipeline.

Create a file <tt>Jenkinsfile</tt> at the top level of the repository
and put the following contents in it:

    /* Change 'any' to the Jenkins slave you would like the
       steps of this pipeline to run on, if you have a preference. */

    def nodeLabel = 'any'

    /* The main pipeline. We'll have a build stage, a test stage, and
       a deploy stage. */

    pipeline {
      agent none
      options {
        buildDiscarder(logRotator(numToKeepStr: '5'))
      }
      stages {
    
        stage('build') {
          /* We can specify any environment for any stage. Here we'll
             use the dev environment to do the initial build. */
          agent {
            dockerfile {
              label "${nodeLabel}"
              filename 'dev/Dockerfile.dev'
              dir 'dev'
            }
          }
          steps {
            /* These steps are going to run inside the dev environment
               container, with the git repo workspace mounted as a bind
               volume. */
            sh '''
              echo "Hello from the build step!"
            '''
          }
        }
    
        stage('test') {
          /* We don't have a test environment yet so we'll run within
             Jenkins' environment for now. */
          agent {
            label "${nodeLabel}"
          }
          steps {
            sh '''
              echo "Hello from the test step!"
            '''
          }
        }
    
        stage('deploy') {
          /* We don't have a deploy environment yet so we'll run within
             Jenkins' environment for now. */
          agent {
            label "${nodeLabel}"
          }
          steps {
            sh '''
              echo "Hello from the deploy step!"
            '''
          }
        }
      }
    }

Next let's go to Jenkins and point it to our project. Log in with your
admin account credentials and click on "New Item." Enter an item name
such as <tt>devops\_workshop</tt>, select "Multibranch pipeline," then
click "OK."

Next you'll see a setup page for the project. Set the Display Name to
the title you want to see in Jenkins' page for this project, then under
Branch Sources, click "Add Source."

In real life, as mentioned earlier, Jenkins would be running on a server
and you'd probably point it to your centralized Git repository. However,
today, we'll just point our local Jenkins to our local Git repository.
Since you already gave your Jenkins container access to your local
repository, you should be able to 
enter <tt>file:///devops\_workshop</tt> for the repository URL.

There's a slightly more complete example in the
