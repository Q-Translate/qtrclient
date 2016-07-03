<?php
/**
 * @file
 * Function translateform_meta()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * When there is only a single verse displayed, we can add metatags
 * about the verse, social share buttons, discussions/comments, etc.
 */
function translateform_meta($lng, $vid, $verse) {
  $form = array();

  // Get verse properties: title, url, description, hashtags
  $properties = _get_verse_properties($verse);
  $properties['lng'] = $lng;

  // Set the page title.
  drupal_set_title($properties['title']);

  // Add metatags: og:title, og:description, og:image, etc.
  _add_metatags($properties, $lng);

  // Add RRSSB share buttons.
  $form['rrssb'] = array(
    '#markup' => srrssb(
      [
        'buttons' => ['googleplus', 'linkedin', 'facebook', 'twitter', 'email'],
        'url' => $properties['url'],
        'title' => $properties['title'],
        'summary' => $properties['description'],
        'hashtags' => $properties['hashtags'],
        'lng' => $properties['lng'],
      ]),
  );

  if (module_exists('disqus')) {
    // Define the disqus form element.
    $form['disqus'] = array(
      '#type' => 'disqus',
      '#disqus' => array(
        'domain' => variable_get('disqus_domain', 'dev-qtranslate'),
        'status' => TRUE,
        'url' => $properties['url'],
        'title' => $properties['title'],
        'identifier' => "translations/$lng/$vid",
        'developer' => variable_get('disqus_developer', '1'),
      ),
      '#weight' => 101,
    );
  }

  return $form;
}

/**
 * Return verse properties as an associative array of:
 * title, url, description, hashtags, etc.
 * These will be used for metatags and for the social share buttons.
 */
function _get_verse_properties($verse) {
  // Get the page title.
  $str = strip_tags(check_plain($verse['verse']));
  $title = t('Verse') . ': ' . $str;
  $title = qcl::shorten($title, 50);

  // Get the description.
  $description = $str;
  $arr_translations = array();
  foreach ($verse['translations'] as $trans) {
    $arr_translations[] = strip_tags(check_plain($trans['translation']));
  }
  if (!empty($arr_translations)) {
    $description .= ' <==> ' . implode(' / ', $arr_translations);
  }
  if (isset($_GET['url'])) {
    $description .= ' See: ' . check_url($_GET['url']);
  }

  // Get the url.
  $uri = substr(request_uri(), 1);
  $url = url($uri, ['absolute' => TRUE]);

  // Get hashtags.
  $arr_tags = array();
  $hashtags = implode(' ', $arr_tags);

  // Return properties.
  $properties = array(
    'title' => $title,
    'url' => $url,
    'description' => $description,
    'hashtags' => $hashtags,
  );
  return $properties;
}

/**
 * Add metatags: og:title, og:description, og:image, og:image:size, etc.
 */
function _add_metatags($properties, $lng) {
  // Add og:type
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:type",
      "content" => "website",
    ),
  );
  drupal_add_html_head($element, 'og_type');

  // Add og:title
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:title",
      "content" => $properties['title'],
    ),
  );
  drupal_add_html_head($element, 'og_title');

  // Add og:description
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:description",
      "content" => $properties['description'],
    ),
  );
  drupal_add_html_head($element, 'og_description');

  // Add og:url
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:url",
      "content" => $properties['url'],
    ),
  );
  drupal_add_html_head($element, 'og_url');

  // Add og:image
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:image",
      "content" => url('logo.png', ['absolute' => TRUE]),
    ),
  );
  drupal_add_html_head($element, 'og_image');

  // Add og:image:width
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "og:image:width",
      "content" => '64',
    ),
  );
  drupal_add_html_head($element, 'og_image_width');

  // twitter:card
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "twitter:card",
      "content" => 'Summary',
    ),
  );
  drupal_add_html_head($element, 'twitter_card');

  // twitter:site
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "twitter:site",
      "content" => '@qtr_' . $lng,
    ),
  );
  drupal_add_html_head($element, 'twitter_site');

  // twitter:title
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "twitter:title",
      "content" => $properties['title'],
    ),
  );
  drupal_add_html_head($element, 'twitter_title');

  // twitter:description
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      "property" => "twitter:description",
      "content" => $properties['description'],
    ),
  );
  drupal_add_html_head($element, 'twitter_description');
}
