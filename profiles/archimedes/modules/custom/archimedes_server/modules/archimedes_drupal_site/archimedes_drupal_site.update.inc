<?php

// Project is missing security update(s).
define('UPDATE_NOT_SECURE', 1);

// Current release has been unpublished and is no longer available.
define('UPDATE_REVOKED', 2);

// Current release is no longer supported by the project maintainer.
define('UPDATE_NOT_SUPPORTED', 3);

// Project has a new release available, but it is not a security release.
define('UPDATE_NOT_CURRENT', 4);

// Project is up to date.
define('UPDATE_CURRENT', 5);

// Project's status cannot be checked.
define('UPDATE_NOT_CHECKED', -1);

// No available update data was found for project.
define('UPDATE_UNKNOWN', -2);

/**
  * Get project information
  */
function archimedes_drupal_site_get_projects($modules)  {
  $projects = array();
  foreach($modules as $module)  {
    $proj_title = $module->field_dru_proj[0]['value'];
    if(!$proj_title) $proj_title = $module->title;

    //only need to add this information once.
    if($proj_title && !array_key_exists($proj_title,$projects))  {
      $cached_data = archimedes_drupal_site_archimedes_cache_get($module);
      //default type is module
      if($proj_title == 'drupal') {
        $proj_type = 'core';
      }
      elseif($module->type == 'drupal_theme') {
        $proj_type = 'theme';
      }
      else{
        $proj_type = 'module';
      }
      $projects[$proj_title] = array(
        'name' => $proj_title,
        'info' => array(
          'name' => $cached_data->data['title'],
          'package' => $module->field_dru_pkg[0]['value'],
          'version' => $module->field_mod_version[0]['value'],
          'project' => $proj_title,
          'datestamp' => $module->created,
        ),
        'project_type' => $proj_type,
        'project_link' => $cached_data->data['link'],
      );
    }
    $cur_version = $projects[$proj_title]['info']['version'];
    $mod_version = $module->field_mod_version[0]['value'];
    if($cur_version < $mod_version){
      $projects[$proj_title]['info']['version'] = $mod_version;
    }
    $projects[$proj_title]['includes'][$module->title] = l($module->title, "node/" . $module->nid);
  }

  return $projects;
}


/**
  * Get cached data about a module
  */
function archimedes_drupal_site_archimedes_cache_get($module){
  $data = array();
  $title = $module->field_dru_proj[0]['value'];
  $version = $module->field_mod_version[0]['value'];
  $exp = explode('.', $version);
  $version = $exp[0] . '.x';
  $key = $title . $version;
  $data = cache_get($key, 'cache_archimedes_drupal');
  return $data;
}

/**
  * Giveen the installed projects and the available release data retrieved from
  * remote servers, calculate the current status.
  *
  * Copied and modified from update_calculate_project_data
  */
function archimedes_drupal_site_calculate_project_data($available, $modules) {
  module_load_include('inc', 'update', 'update.compare');
  $projects = archimedes_drupal_site_get_projects($modules);
  update_process_project_info($projects);
  foreach ($projects as $project => $project_info) {
    if (isset($available[$project])) {
      // If the project status is marked as something bad, there's nothing
      // else to consider.
      if (isset($available[$project]['project_status'])) {
        switch ($available[$project]['project_status']) {
          case 'insecure':
            $projects[$project]['status'] = UPDATE_NOT_SECURE;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'project-not-secure',
              'label' => t('Project not secure'),
              'data' => t('This project has been labeled insecure by the Drupal security team, and is no longer available for download. Immediately disabling everything included by this project is strongly recommended!'),
            );
            break;
          case 'unpublished':
          case 'revoked':
            $projects[$project]['status'] = UPDATE_REVOKED;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'project-revoked',
              'label' => t('Project revoked'),
              'data' => t('This project has been revoked, and is no longer available for download. Disabling everything included by this project is strongly recommended!'),
            );
            break;
          case 'unsupported':
            $projects[$project]['status'] = UPDATE_NOT_SUPPORTED;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'project-not-supported',
              'label' => t('Project not supported'),
              'data' => t('This project is no longer supported, and is no longer available for download. Disabling everything included by this project is strongly recommended!'),
            );
            break;
          case 'not-fetched':
            $projects[$project]['status'] = UPDATE_NOT_FETCHED;
            $projects[$project]['reason'] = t('Failed to fetch available update data');
            break;

          default:
            // Assume anything else (e.g. 'published') is valid and we should
            // perform the rest of the logic in this function.
            break;
        }
      }

      if (!empty($projects[$project]['status'])) {
        // We already know the status for this project, so there's nothing
        // else to compute. Just record everything else we fetched from the
        // XML file into our projects array and move to the next project.
        $projects[$project] += $available[$project];
        continue;
      }

      // Figure out the target major version.
      $existing_major = $project_info['existing_major'];
      $supported_majors = array();
      if (isset($available[$project]['supported_majors'])) {
        $supported_majors = explode(',', $available[$project]['supported_majors']);
      }
      elseif (isset($available[$project]['default_major'])) {
        // Older release history XML file without supported or recommended.
        $supported_majors[] = $available[$project]['default_major'];
      }

      if (in_array($existing_major, $supported_majors)) {
        // Still supported, stay at the current major version.
        $target_major = $existing_major;
      }
      elseif (isset($available[$project]['recommended_major'])) {
        // Since 'recommended_major' is defined, we know this is the new XML
        // format. Therefore, we know the current release is unsupported since
        // its major version was not in the 'supported_majors' list. We should
        // find the best release from the recommended major version.
        $target_major = $available[$project]['recommended_major'];
        $projects[$project]['status'] = UPDATE_NOT_SUPPORTED;
      }
      elseif (isset($available[$project]['default_major'])) {
        // Older release history XML file without recommended, so recommend
        // the currently defined "default_major" version.
        $target_major = $available[$project]['default_major'];
      }
      else {
        // Malformed XML file? Stick with the current version.
        $target_major = $existing_major;
      }

      // Make sure we never tell the admin to downgrade. If we recommended an
      // earlier version than the one they're running, they'd face an
      // impossible data migration problem, since Drupal never supports a DB
      // downgrade path. In the unfortunate case that what they're running is
      // unsupported, and there's nothing newer for them to upgrade to, we
      // can't print out a "Recommended version", but just have to tell them
      // what they have is unsupported and let them figure it out.
      $target_major = max($existing_major, $target_major);

      $version_patch_changed = '';
      $patch = '';

      // Defend ourselves from XML history files that contain no releases.
      if (empty($available[$project]['releases'])) {
        $projects[$project]['status'] = UPDATE_UNKNOWN;
        $projects[$project]['reason'] = t('No available releases found');
        continue;
      }
      foreach ($available[$project]['releases'] as $version => $release) {
        // First, if this is the existing release, check a few conditions.
        if ($projects[$project]['existing_version'] === $version) {
          if (isset($release['terms']['Release type']) &&
              in_array('Insecure', $release['terms']['Release type'])) {
            $projects[$project]['status'] = UPDATE_NOT_SECURE;
          }
          elseif ($release['status'] == 'unpublished') {
            $projects[$project]['status'] = UPDATE_REVOKED;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'release-revoked',
              'label' => t('Release revoked'),
              'data' => t('Your currently installed release has been revoked, and is no longer available for download. Disabling everything included in this release or upgrading is strongly recommended!'),
            );
          }
          elseif (isset($release['terms']['Release type']) &&
                  in_array('Unsupported', $release['terms']['Release type'])) {
            $projects[$project]['status'] = UPDATE_NOT_SUPPORTED;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'release-not-supported',
              'label' => t('Release not supported'),
              'data' => t('Your currently installed release is now unsupported, and is no longer available for download. Disabling everything included in this release or upgrading is strongly recommended!'),
            );
          }
        }

        // Otherwise, ignore unpublished, insecure, or unsupported releases.
        if ($release['status'] == 'unpublished' ||
            (isset($release['terms']['Release type']) &&
             (in_array('Insecure', $release['terms']['Release type']) ||
              in_array('Unsupported', $release['terms']['Release type'])))) {
          continue;
        }

        // See if this is a higher major version than our target and yet still
        // supported. If so, record it as an "Also available" release.
        if ($release['version_major'] > $target_major) {
          if (in_array($release['version_major'], $supported_majors)) {
            if (!isset($available[$project]['also'])) {
              $available[$project]['also'] = array();
            }
            if (!isset($available[$project]['also'][$release['version_major']])) {
              $available[$project]['also'][$release['version_major']] = $version;
            }
          }
          // Otherwise, this release can't matter to us, since it's neither
          // from the release series we're currently using nor the recommended
          // release. We don't even care about security updates for this
          // branch, since if a project maintainer puts out a security release
          // at a higher major version and not at the lower major version,
          // they must remove the lower version from the supported major
          // versions at the same time, in which case we won't hit this code.
          continue;
        }

        // Look for the 'latest version' if we haven't found it yet. Latest is
        // defined as the most recent version for the target major version.
        if (!isset($available[$project]['latest_version'])
             && $release['version_major'] == $target_major) {
          $available[$project]['latest_version'] = $version;
        }

        // Look for the development snapshot release for this branch.
        if (!isset($available[$project]['dev_version'])
             && $release['version_major'] == $target_major
             && isset($release['version_extra'])
             && $release['version_extra'] == 'dev') {
          $available[$project]['dev_version'] = $version;
        }

        // Look for the 'recommended' version if we haven't found it yet (see
        // phpdoc at the top of this function for the definition).
        if (!isset($available[$project]['recommended'])
             && $release['version_major'] == $target_major
             && isset($release['version_patch'])) {
          if ($patch != $release['version_patch']) {
            $patch = $release['version_patch'];
            $version_patch_changed = $release['version'];
          }
          if (empty($release['version_extra']) && $patch == $release['version_patch']) {
            $available[$project]['recommended'] = $version_patch_changed;
          }
        }

        // Stop searching once we hit the currently installed version.
        if ($projects[$project]['existing_version'] === $version) {
          break;
        }

        // If we're running a dev snapshot and have a timestamp, stop
        // searching for security updates once we hit an official release
        // older than what we've got. Allow 100 seconds of leeway to handle
        // differences between the datestamp in the .info file and the
        // timestamp of the tarball itself (which are usually off by 1 or 2
        // seconds) so that we don't flag that as a new release.
        if ($projects[$project]['install_type'] == 'dev') {
          if (empty($projects[$project]['datestamp'])) {
            // We don't have current timestamp info, so we can't know.
            continue;
          }
          elseif (isset($release['date']) && ($projects[$project]['datestamp'] + 100 > $release['date'])) {
            // We're newer than this, so we can skip it.
            continue;
          }
        }

        // See if this release is a security update.
        if (isset($release['terms']['Release type'])
             && in_array('Security update', $release['terms']['Release type'])) {
          $projects[$project]['security updates'][] = $release;
        }
      }

      // If we were unable to find a recommended version, then make the latest
      // version the recommended version if possible.
      if (!isset($available[$project]['recommended']) && isset($available[$project]['latest_version'])) {
        $available[$project]['recommended'] = $available[$project]['latest_version'];
      }

      // Stash the info about available releases into our $projects array.
      $projects[$project] += $available[$project];

      //
      // Check to see if we need an update or not.
      //

      if (!empty($projects[$project]['security updates'])) {
        // If we found security updates, that always trumps any other status.
        $projects[$project]['status'] = UPDATE_NOT_SECURE;
      }

      if (isset($projects[$project]['status'])) {
        // If we already know the status, we're done.
        continue;
      }

      // If we don't know what to recommend, there's nothing we can report.
      // Bail out early.
      if (!isset($projects[$project]['recommended'])) {
        $projects[$project]['status'] = UPDATE_UNKNOWN;
        $projects[$project]['reason'] = t('No available releases found');
        continue;
      }

      // If we're running a dev snapshot, compare the date of the dev snapshot
      // with the latest official version, and record the absolute latest in
      // 'latest_dev' so we can correctly decide if there's a newer release
      // than our current snapshot.
      if ($projects[$project]['install_type'] == 'dev') {
        if (isset($available[$project]['dev_version']) && $available[$project]['releases'][$available[$project]['dev_version']]['date'] > $available[$project]['releases'][$available[$project]['latest_version']]['date']) {
          $projects[$project]['latest_dev'] = $available[$project]['dev_version'];
        }
        else {
          $projects[$project]['latest_dev'] = $available[$project]['latest_version'];
        }
      }

      // Figure out the status, based on what we've seen and the install type.
      switch ($projects[$project]['install_type']) {
        case 'official':
          if ($projects[$project]['existing_version'] === $projects[$project]['recommended'] || $projects[$project]['existing_version'] === $projects[$project]['latest_version']) {
            $projects[$project]['status'] = UPDATE_CURRENT;
          }
          else {
            $projects[$project]['status'] = UPDATE_NOT_CURRENT;
          }
          break;

        case 'dev':
          $latest = $available[$project]['releases'][$projects[$project]['latest_dev']];
          if (empty($projects[$project]['datestamp'])) {
            $projects[$project]['status'] = UPDATE_NOT_CHECKED;
            $projects[$project]['reason'] = t('Unknown release date');
          }
          elseif (($projects[$project]['datestamp'] + 100 > $latest['date'])) {
            $projects[$project]['status'] = UPDATE_CURRENT;
          }
          else {
            $projects[$project]['status'] = UPDATE_NOT_CURRENT;
          }
          break;

        default:
          $projects[$project]['status'] = UPDATE_UNKNOWN;
          $projects[$project]['reason'] = t('Invalid info');
      }
    }
    else {
      $projects[$project]['status'] = UPDATE_UNKNOWN;
      $projects[$project]['reason'] = t('No available releases found');
    }
  }

  return $projects;
}
