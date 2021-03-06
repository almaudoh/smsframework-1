<?php

/**
 * @file
 * Message tracking module: Admin settings form functions.
 *
 * @package sms
 * @subpackage sms_track
 */

/**
 * Admin settings form.
 *
 * Form builder for sms_track settings form.
 *
 * @return
 *   Backdrop form array.
 *
 * @see sms_track_settings_form_submit()
 */

function sms_track_settings_form() {
  $config = config('sms.track.settings');
  
  // Archive section.
  $form['archive'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Message archiving',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['archive']['archive_dir'] = array(
    '#type'  => 'select',
    '#title' => 'Archive mode',
    '#default_value' => $config->get('sms_track_archive_dir'),
    '#options' => array(
      SMS_DIR_NONE => 'No archiving [default]',
      SMS_DIR_OUT  => 'Record outgoing messages only',
      SMS_DIR_IN   => 'Record incoming messages only',
      SMS_DIR_ALL  => 'Record both outgoing and incoming messages',
    ),
    '#description' => t('Note that this will revert to the default option when the SMS Tracking module is disabled.'),
  );
  $form['archive']['archive_max_age_days'] = array(
    '#type'  => 'textfield',
    '#title' => 'Purge messages after n days',
    '#size'          => 3,
    '#maxlength'     => 3,
    '#default_value' => $config->get('sms_track_archive_max_age_days'),
    '#description'   => 'Set to 0 (zero) to disable archive purge. This will only work if you have cron configured correctly.',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for sms_track_settings_form.
 *
 * @see sms_track_settings_form
 */
function sms_track_settings_form_submit($form, &$form_state) {
  $config = config('sms.track.settings');

  $archive_dir_old = $config->get('sms_track_archive_dir');
  $archive_dir = $form_state['values']['archive_dir'];
  $config->set('sms_track_archive_dir', $archive_dir);

  $archive_max_age_days = $form_state['values']['archive_max_age_days'];
  $config->set('sms_track_archive_max_age_days', $archive_max_age_days);
  $config->save();

  // Trigger watchdog messages.
  if ($archive_dir_old && ! $archive_dir) {
    watchdog('sms_track', 'SMS Tracking archive collector DISABLED');
  }
  if (! $archive_dir_old && $archive_dir) {
    watchdog('sms_track', 'SMS Tracking archive collector enabled');
  }

  backdrop_set_message(t('Settings saved.'));
}


/**
 * Provides the admin view page from the sms_track view in database.
 *
 * @return string
 *   HTML content string.
 */
function sms_track_admin_view() {
  $content = views_embed_view('sms_track', 'view_all_block');
  return $content;
}
