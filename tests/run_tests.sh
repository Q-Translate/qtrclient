#!/bin/bash
### Get the required modules and libraries on the profile 'testing',
### then run the tests.

cd $(dirname $0)

### get the paths
drupal_dir=$(drush php-eval 'print realpath(".")')
testing_dir=$drupal_dir/profiles/testing

### create a make file
cat <<EOF  > $testing_dir/testing.make
api = 2
core = 7.x

defaults[projects][subdir] = "contrib"

projects[qtrClient][type] = "module"
projects[qtrClient][subdir] = "custom"
projects[qtrClient][download][type] = "git"
projects[qtrClient][download][url] = "https://github.com/Q-Translate/qtrClient.git"
;projects[qtrClient][download][branch] = "7.x-1.x"
EOF

### get the dependencies
drush dl drush_remake --no
drush remake testing

### make sure that simpletest is enabled
drush dl simpletest --no
drush en simpletest --yes

### run the tests
drush test-clean
drush test-run QtrClientTestCase
