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
function translateform_meta($lng, $verse) {
  $form = array();

  // Get verse properties: title, url, description, hashtags
  $properties = _get_verse_properties($lng, $verse);

  // Set the page title.
  drupal_set_title($properties['title']);

  // Add metatags: og:title, og:description, og:image, etc.
  _add_metatags($properties);

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

  // Add referencies to related sites.
  $form += _referencies($lng, $verse);

  if (module_exists('disqus')) {
    // Define the disqus form element.
    $cid = $verse['cid'];
    $nr = $verse['nr'];
    $form['disqus'] = array(
      '#type' => 'disqus',
      '#disqus' => array(
        'domain' => variable_get('disqus_domain', 'dev-qtranslate'),
        'status' => TRUE,
        'url' => $properties['url'],
        'title' => $properties['title'],
        'identifier' => "qtr/$lng/$cid/$nr",
        'developer' => variable_get('disqus_developer', '1'),
      ),
      '#weight' => 101,
    );
  }

  return $form;
}

/**
 * Return referencies related to the current verse.
 */
function _referencies($lng, $verse) {
  // Display/hide related references.
  $form['references'] = [
    '#markup' => '
        <div class="btn-group" data-toggle="buttons" style="margin-top: 5px;">
          <label class="btn btn-default">
            <input type="checkbox" id="edit-references" name="references">
            <span class="glyphicon glyphicon-share-alt"></span>
          </label>
        </div>
    ',
  ];

  // List of related sites.
  $chapter_id = $verse['cid'];
  $verse_nr = $verse['nr'];
  $reference_list = [
    'reference1' => [
      'label' => 'quran.com',
      'url' => "https://quran.com/$chapter_id/$verse_nr",
    ],
    'reference2' => [
      'label' => 'quranwow.com',
      'url' => "http://www.quranwow.com/#/ch/$chapter_id/t1/ar-allah/t2/none/a1/alafasy-64/a2/sahihinternational-64/v/$verse_nr",
    ],
    'reference3' => [
      'label' => 'tanzil.net',
      'url' => "http://tanzil.net/#$chapter_id:$verse_nr",
    ],
  ];
  $form['related-references'] = [
    '#type' => 'fieldset',
    '#states' => ['visible' => [':input[name="references"]' => ['checked' => TRUE],]],
    '#attributes' => ['style' => 'float: right; border: none; margin: 0px;'],
  ];
  foreach ($reference_list as $key => $reference) {
    $url = $reference['url'];
    $label = $reference['label'];
    $form['related-references'][$key] = [
      '#markup' => "
          <a href='$url' class='btn btn-default btn-sm' target='_blank'>
            <span class='glyphicon glyphicon-link'></span> $label
          </a>
      ",
    ];
  }

  return $form;
}

/**
 * Return verse properties as an associative array of:
 * title, url, description, hashtags, etc.
 * These will be used for metatags and for the social share buttons.
 */
function _get_verse_properties($lng, $verse) {
  // Get the page title.
  $cid = $verse['cid'];
  $nr = $verse['nr'];
  $str = strip_tags(check_plain($verse['translations'][0]['translation']));
  $title = "Q-Translate $lng/$cid/$nr: " . $str;
  $title = qcl::shorten($title, 50);

  // Get the description.
  $description = $verse['verse'];
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

  // Return properties.
  $properties = array(
    'lng' => $lng,
    'title' => $title,
    'url' => $url,
    'description' => $description,
    'hashtags' => "#qtr_$lng",
  );
  return $properties;
}

/**
 * Add metatags: og:title, og:description, og:image, og:image:size, etc.
 */
function _add_metatags($properties) {
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
