<?php

/**
 * @file get.pages.inc.
 * Configuration form to create a new Get Importers.
 */

 /**
  * Provide the configuration form for new Get instances.
  */
function get_create_form(array $form, array &$form_state) {
  $form = [];
  $form['help'] = [
    '#markup' => '<div class="get-help-text">
        Each <em>Get Importer</em> provides an interface for polling a data API,
        retrieving the data, parsing and saving the data into Backdrop nodes or
        entities.
      </div>',
  ];
  $form['name'] = [
    '#title' => t('Name'),
    '#description' => t('Provide a name for the Get Importer.'),
    '#type' => 'textfield',
    '#size' => 33,
  ];
  $form['desctription'] = [
    '#type' => 'textfield',
    '#title' => t('Descripton'),
    '#description' => t('Describe the Get Importer; i.e. get JSON data from NASA API.'),
    '#size' => 66,
  ];
  $form['source'] = [
    '#type' => 'textfield',
    '#title' => t('Source URL'),
    '#description' => t('The source URL for the data.'),
    '#size' => 66
  ];
  $targets = entity_get_info();
  $options = [];
  $options['select-one'] = '-- Select --';
  foreach($targets as $target) {
    foreach($target['bundles'] as $bundle) {
      $key = $target['base table'] . "_" . $bundle['label'];
      $value = $bundle['label'];
      $options[$key] = $value;
    }
  }
  //dpm($options);
  $form['target'] = [
    '#type' => 'select',
    '#title' => t('Target'),
    '#options' => $options,
    '#description' => t('The Entity type that you wish to store
      the data form the Get Import.'
    ),
    '#ajax' => [
      'callback' => 'map_choices',
      'wrapper' => 'map-choices',
      'method' => 'replace',
      'effect' => 'fade',
    ],
  ];
  $form['target-choices'] = [
    '#type' => 'fieldset',
    '#title' => t('Target fields'),
    '#prefix' => '<div id="map-choices">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];
  $form['source-fields'] = [
    '#type' => 'select',
    '#title' => 'Source field',
    '#options' => $source_fields,
    '#description' => 'Select a field to add to the map.',
  ];

  return $form;
}

/**
 * Ajax callback for target bundle to give target field map.
 */
function map_choices(&$form, &$form_state) {
  // Get the source data fields.
  $url = $form_state['values']['source'];
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
  $content = curl_exec($ch);
  curl_close($ch);
  $json = json_decode($content);
  $source_options = '<strong>Source Options</strong><br />';
  $source_fields = [];
  foreach($json[0] as $key => $j) {
    $source_options .= "<p>$key</p>";
    $source_fields[$key] = $key;
  }

  // Get the possible field targets.
  $type_bundle = $form_state['values']['target'];
  $type_bundle = explode('_', $type_bundle);
  $type = $type_bundle[0];
  $bundle = strtolower($type_bundle[1]);
  $fields = field_info_instances($type, $bundle);

  // Prepare the markup.
  $markup = "<div id='get-map'>";
  $markup .= "<div id='source-choices'>";
  $markup .= $source_options;
  // end of source-choices div.
  $markup .= "</div>";
  $markup .= "<div id='target-choices'>";
  $markup .= "<strong>Target Options</strong><br />";
  $markup .= "<p>title</p>";
  foreach ($fields as $field) {
    $target_field = $field['field_name'];
    $markup .= "<p>$target_field</p>";
  }
  // end of target-choices div.
  $markup .= "</div>";
  // end of get-map div.
  $markup .= "</div>";

  return $markup;
}

/**
 * Page callback for /admin/structure/get/manage.
 */
function get_manage_form() {
  $form = [];
  $form['help'] = [
    '#markup' => '<div class="get-help-text">
      Manage Get Importers.
    </div>',
  ];
  return $form;
}
