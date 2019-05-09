
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
          ./scripts/build.sh
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


