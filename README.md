
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

### Get Jenkins up and running

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

The second volume option links the Docker daemon's socket inside
the container to the one on your host. This combined with the
<tt>--priviledged</tt> option should enable your Jenkins instance
to talk to your Docker daemon. However, you'll have to make a small fix
as described below to complete the connection.

Anyway,
the third volume option mounts your local copy of of the
git repository to the path <tt>/devops\_workshop</tt> inside Jenkins'
Docker environment. In a bit, we'll have Jenkins monitor your repository
via this bind mount. The directory is mounted read only for safety.

You can use a different host port instead of 8080 if you've already got
something like Tomcat running on port 8080.

OK, copy the initial setup password then visit
<a href="http://localhost:8080">http://localhost:8080</a> in a browser.
Use the given password, click on "Install suggested plugins" and wait
for the plugins to install. It'll probably take a few minutes.

### Hack to allow Jenkins to run Docker-in-Docker!

This is a bit of a hack and only necessary because Jenkins is running
in a Docker container AND we want Jenkins itself to run Docker to
create our build/test/deploy environments.

First, make sure that on the host, the user with ID 1000 is able to
run Docker commands.  To find out which user has user ID 1000,
take a look at <tt>/etc/passwd</tt>.  If that user can run <tt>docker ps</tt>
and so on, all is well. If not, you may need to add that user to the
Docker group with a line in /etc/group such as

    docker:x:135:user1,user2,user3

where one of the users listed is user 1000.

Next, once you've started the Jenkins container with the
<tt>docker run</tt> command above, you need to make sure that the Jenkins
user inside the Jenkins container also has access to Docker, WITHIN THE
DOCKER CONTAINER!! To do this, do a

    $ docker ps

to find out your Jenkins container ID, then run

    host$ docker run -it containerId bash
    cont$ su         # Password is 12345 you might want to change it!
    cont# chown root.docker /var/run/docker.sock
    cont# exit
    cont$ docker ps  # Should work OK

If anyone has a foolproof method to set up an Jenkins-with-Docker image
that avoids this step, please let us know!

### First pipeline

Jenkins is problably still downloading plugins.
While that's going on, let's create our first build pipeline.

Create a file <tt>Jenkinsfile</tt> at the top level of the repository
and put the following contents in it:

    /* Change 'master' to the Jenkins slave you would like the
       steps of this pipeline to run on, if you have a preference. */

    def nodeLabel = 'master'

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
              filename 'Dockerfile.dev'
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

You may want to initially specify that Jenkins should only build the
<tt>before-test</tt> branch rather than scan for all branches with a
Jenkinsfile.

Finally, Jenkins should scan for your repo and start building. If
Docker is
set up properly, you should get a successful build. If not, look at
the console log to see what went wrong.

## The build stage

OK, now that we have a basic pipeline running without doing much,
let's fully implement the "build" stage of our pipeline.

Generally, we want to keep as much application-specific logic out
of the Jenkins pipeline and do everything we can with scripts
that are called from the pipeline.

In the current case, we want to run the build script (which is already
in the <tt>scripts/</tt> directory) then <i>archive the artifacts</i>
(executables, libraries, etc.)
that will be used in the rest of the pipeline.

One important CI/CD principle is that we only build artifacts once then
use those artifacts in the later stages of the pipeline.

Now this may not always be possible; for example, when we do unit testing
we normally use a separate test framework that should not be included in
the production/release version.

However, that said, we'll build release/production binaries now then
use them rather than rebuild then where appropriate later.

So! Change the dummy build stage contents to the following:

    steps {
      /* These steps are going to run inside the dev environment
         container, with the git repo workspace mounted as a bind
         volume. */
      sh '''
        ./scripts/build.sh
        mkdir -p dist
        cp facedetection_va/cmake-build-release/facedetector dist/
      '''
      archiveArtifacts artifacts: 'dist/*', fingerprint: true
    }

Once your build is successful, check the branch build page
(<a href=http://localhost:8080/job/devops_workshop/job/before-test>http://localhost:8080/job/devops_workshop/job/before-test></a>)
and verify that the artifacts are successfully published and downloadable.

If you have any trouble with this part of the tutorial,
check the <tt>02-pipeline</tt> branch of the original repository and you'll
see our setup.

## The test stage

Now that we have a basic build stage in place, let's get the test
stage doing something useful for us.

Try the following to switch to the
<tt>03-test</tt> branch of the repository and run
tests:

    host$ git checkout 03-test
    host$ docker ps
    host$ # find the CONTAINER ID for the dev environment. Suppose it's containerId.
    host$ docker run -it containerId bash
    cont# cd /home/dev/devops_workshop
    cont# ./scripts/test.sh

Does it work? Hope so!  It should run test suites for the Web application
and the face detector service and record code coverage reports for both
sets of tests.

Now our task is to execute the tests in our CI/CD pipeline.

