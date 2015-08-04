<?php
/**
 * @file
 * Function rrssb_get_buttons()
 *
 * Don't forget to add on hook_init():
 *   drupal_add_css($path . '/lib/fn/rrssb/rrssb.css');
 *   drupal_add_js($path . '/lib/fn/rrssb/rrssb.min.js');
 */

namespace BTranslator\Client;
use \bcl;

/**
 * Returns an html list of the themed links with urls.
 *
 * @param array $options
 *   Associtive array of the options:
 *     - buttons
 *     - url
 *     - title
 *     - summary
 *     - hashtags
 *     - lng
 *     - pinterest_media
 *     - youtube_username
 *     - github_username
 *
 * @return string
 *   A string of markup for the list of buttons.
 */
function rrssb_get_buttons($options = array()) {
  // Get the options.
  $options += [
    'buttons' => ['googleplus', 'linkedin', 'facebook', 'twitter', 'github'],
    'url' => url(request_uri(), ['absolute' => TRUE]),
    'title' => drupal_get_title(),
    'summary' => '',
    'hashtags' => '',
    'lng' => variable_get('btrClient_translation_lng', 'fr'),
    'pinterest_media' => NULL,
    'youtube_username' => NULL,
    'github_username' => NULL,
  ];
  $options['url'] = rawurlencode($options['url']);
  $options['title'] = rawurlencode($options['title']);
  $options['summary'] = rawurlencode($options['summary']);
  $options['hashtags'] = rawurlencode($options['hashtags']);
  $options['pinterest_media'] = rawurlencode($options['pinterest_media']);

  // Get the url for each button.
  $buttons = array();
  for ($i = 0; $i < sizeof($options['buttons']); $i++) {
    $button = $options['buttons'][$i];
    $buttons[$button] =_get_button_url($button, $options);
  }

  // Build the HTML code of the list.
  $output = '<div class="rrssb-container clearfix">
    <ul class="rrssb-buttons">
  ';
  foreach ($buttons as $button => $url) {
    $output .= _get_button_code($button, $url);
  }
  $output .= '</ul></div>';

  return $output;
}

/**
 * Return the url of a button.
 */
function _get_button_url($button, $options) {
  switch ($button) {

    // email
    case 'email':
      $body = $options['url'] . rawurlencode("\n\n") . $options['summary'];
      return 'mailto:?subject=' . $options['title'] . '&amp;body=' . $body;
      break;

      // facebook
    case 'facebook':
      return  'https://www.facebook.com/sharer/sharer.php?u=' . $options['url'];
      break;

      // linkedin
    case 'linkedin':
      return 'http://www.linkedin.com/shareArticle?mini=true&amp;url=' . $options['url'] .'&amp;title=' . $options['title'] . '&amp;summary=' . $options['summary'];
      break;

      // twitter
    case 'twitter':
      $link = rawurldecode($options['url']);
      $hashtags = rawurldecode($options['hashtags']);
      $summary = rawurldecode($options['summary']);
      $summary = bcl::shorten($summary, 115 - strlen($hashtags));
      $status = rawurlencode($summary . ' ' . $link . ' ' . $hashtags);
      return 'http://twitter.com/home?status=' . $status;
      break;

      // googleplus
    case 'googleplus':
      return 'https://plus.google.com/share?hl=' . $options['lng'] .'&amp;url=' . $options['title'] . '%20' . $options['url'];
      break;

      // pinterest
    case 'pinterest':
      return 'http://pinterest.com/pin/create/button/?url=' . $options['url'] . '&amp;media=' . $options['pinterest_media'] .'&amp;description=' . $options['title'];
      break;

      // tumblr
    case 'tumblr':
      return 'http://tumblr.com/share?s=&amp;v=3&t='. $options['title']
        . '&amp;u=' . $options['url'];
      break;

      // reddit
    case 'reddit':
      return 'http://www.reddit.com/submit?url=' . $options['url'];
      break;

      // hackernews
    case 'hackernews':
      return 'https://news.ycombinator.com/submitlink?u=' . $options['url']
        . '&t=' . $options['title'] . '&text=' . $options['title'];
      break;

      // youtube
    case 'youtube':
      return 'https://www.youtube.com/user/' . $options['youtube_username'];
      break;

      // pocket
    case 'pocket':
      return 'https://getpocket.com/save?url=' . $options['url'];
      break;

      // github
    case 'github':
      return 'https://github.com/'. $options['github_username'];
      break;

      // vk
    case 'vk':
      return 'http://vk.com/share.php?url=' . $options['url'];
      break;
  }
}

/**
 * Return the html code of a button.
 *
 * Based on index.html in the rrssb library.  In particular, the <span> lines
 * are copied verbatim, preserving whitespace from index.html as this makes it
 * faster to updating with new versions.
 */
function _get_button_code($button, $url) {

  switch ($button) {

    // email
    case 'email':
      return '
        <li class="rrssb-email">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M20.11 26.147c-2.335 1.05-4.36 1.4-7.124 1.4C6.524 27.548.84 22.916.84 15.284.84 7.343 6.602.45 15.4.45c6.854 0 11.8 4.7 11.8 11.252 0 5.684-3.193 9.265-7.398 9.3-1.83 0-3.153-.934-3.347-2.997h-.077c-1.208 1.986-2.96 2.997-5.023 2.997-2.532 0-4.36-1.868-4.36-5.062 0-4.75 3.503-9.07 9.11-9.07 1.713 0 3.7.4 4.6.972l-1.17 7.203c-.387 2.298-.115 3.3 1 3.4 1.674 0 3.774-2.102 3.774-6.58 0-5.06-3.27-8.994-9.304-8.994C9.05 2.87 3.83 7.545 3.83 14.97c0 6.5 4.2 10.2 10 10.202 1.987 0 4.09-.43 5.647-1.245l.634 2.22zM16.647 10.1c-.31-.078-.7-.155-1.207-.155-2.572 0-4.596 2.53-4.596 5.53 0 1.5.7 2.4 1.9 2.4 1.44 0 2.96-1.83 3.31-4.088l.592-3.72z"
                />
              </svg>
            </span>
            <span class="rrssb-text">email</span>
          </a>
        </li>
      ';
      break;

      // facebook
    case 'facebook':
      return '
        <li class="rrssb-facebook">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" preserveAspectRatio="xMidYMid" width="29" height="29" viewBox="0 0 29 29">
                <path d="M26.4 0H2.6C1.714 0 0 1.715 0 2.6v23.8c0 .884 1.715 2.6 2.6 2.6h12.393V17.988h-3.996v-3.98h3.997v-3.062c0-3.746 2.835-5.97 6.177-5.97 1.6 0 2.444.173 2.845.226v3.792H21.18c-1.817 0-2.156.9-2.156 2.168v2.847h5.045l-.66 3.978h-4.386V29H26.4c.884 0 2.6-1.716 2.6-2.6V2.6c0-.885-1.716-2.6-2.6-2.6z"
                class="cls-2" fill-rule="evenodd" />
              </svg>
            </span>
            <span class="rrssb-text">facebook</span>
          </a>
        </li>
      ';
      break;

      // linkedin
    case 'linkedin':
      return '
        <li class="rrssb-linkedin">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M25.424 15.887v8.447h-4.896v-7.882c0-1.98-.71-3.33-2.48-3.33-1.354 0-2.158.91-2.514 1.802-.13.315-.162.753-.162 1.194v8.216h-4.9s.067-13.35 0-14.73h4.9v2.087c-.01.017-.023.033-.033.05h.032v-.05c.65-1.002 1.812-2.435 4.414-2.435 3.222 0 5.638 2.106 5.638 6.632zM5.348 2.5c-1.676 0-2.772 1.093-2.772 2.54 0 1.42 1.066 2.538 2.717 2.546h.032c1.71 0 2.77-1.132 2.77-2.546C8.056 3.593 7.02 2.5 5.344 2.5h.005zm-2.48 21.834h4.896V9.604H2.867v14.73z"
                />
              </svg>
            </span>
            <span class="rrssb-text">linkedin</span>
          </a>
        </li>
      ';
      break;

      // twitter
    case 'twitter':
      return '
        <li class="rrssb-twitter">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M24.253 8.756C24.69 17.08 18.297 24.182 9.97 24.62c-3.122.162-6.22-.646-8.86-2.32 2.702.18 5.375-.648 7.507-2.32-2.072-.248-3.818-1.662-4.49-3.64.802.13 1.62.077 2.4-.154-2.482-.466-4.312-2.586-4.412-5.11.688.276 1.426.408 2.168.387-2.135-1.65-2.73-4.62-1.394-6.965C5.574 7.816 9.54 9.84 13.802 10.07c-.842-2.738.694-5.64 3.434-6.48 2.018-.624 4.212.043 5.546 1.682 1.186-.213 2.318-.662 3.33-1.317-.386 1.256-1.248 2.312-2.4 2.942 1.048-.106 2.07-.394 3.02-.85-.458 1.182-1.343 2.15-2.48 2.71z"
                />
              </svg>
            </span>
            <span class="rrssb-text">twitter</span>
          </a>
        </li>
      ';
      break;

      // googleplus
    case 'googleplus':
      return '
        <li class="rrssb-googleplus">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M14.703 15.854l-1.22-.948c-.37-.308-.88-.715-.88-1.46 0-.747.51-1.222.95-1.662 1.42-1.12 2.84-2.31 2.84-4.817 0-2.58-1.62-3.937-2.4-4.58h2.098l2.203-1.384h-6.67c-1.83 0-4.467.433-6.398 2.027C3.768 4.287 3.06 6.018 3.06 7.576c0 2.634 2.02 5.328 5.603 5.328.34 0 .71-.033 1.083-.068-.167.408-.336.748-.336 1.324 0 1.04.55 1.685 1.01 2.297-1.523.104-4.37.273-6.466 1.562-1.998 1.187-2.605 2.915-2.605 4.136 0 2.512 2.357 4.84 7.288 4.84 5.822 0 8.904-3.223 8.904-6.41.008-2.327-1.36-3.49-2.83-4.73h-.01zM10.27 11.95c-2.913 0-4.232-3.764-4.232-6.036 0-.884.168-1.797.744-2.51.543-.68 1.49-1.12 2.372-1.12 2.807 0 4.256 3.797 4.256 6.24 0 .613-.067 1.695-.845 2.48-.537.55-1.438.947-2.295.95v-.003zm.032 13.66c-3.62 0-5.957-1.733-5.957-4.143 0-2.408 2.165-3.223 2.91-3.492 1.422-.48 3.25-.545 3.556-.545.34 0 .52 0 .767.034 2.574 1.838 3.706 2.757 3.706 4.48-.002 2.072-1.736 3.664-4.982 3.648l.002.017zM23.254 11.89V8.52H21.57v3.37H18.2v1.714h3.367v3.4h1.684v-3.4h3.4V11.89"
                />
              </svg>
            </span>
            <span class="rrssb-text">google+</span>
          </a>
        </li>
      ';
      break;

      // pinterest
    case 'pinterest':
      return '
        <li class="rrssb-pinterest">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M14.02 1.57c-7.06 0-12.784 5.723-12.784 12.785S6.96 27.14 14.02 27.14c7.062 0 12.786-5.725 12.786-12.785 0-7.06-5.724-12.785-12.785-12.785zm1.24 17.085c-1.16-.09-1.648-.666-2.558-1.22-.5 2.627-1.113 5.146-2.925 6.46-.56-3.972.822-6.952 1.462-10.117-1.094-1.84.13-5.545 2.437-4.632 2.837 1.123-2.458 6.842 1.1 7.557 3.71.744 5.226-6.44 2.924-8.775-3.324-3.374-9.677-.077-8.896 4.754.19 1.178 1.408 1.538.49 3.168-2.13-.472-2.764-2.15-2.683-4.388.132-3.662 3.292-6.227 6.46-6.582 4.008-.448 7.772 1.474 8.29 5.24.58 4.254-1.815 8.864-6.1 8.532v.003z"
                />
              </svg>
            </span>
            <span class="rrssb-text">pinterest</span>
          </a>
        </li>
      ';
      break;

      // tumblr
    case 'tumblr':
      return '
        <li class="rrssb-tumblr">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M18.02 21.842c-2.03.052-2.422-1.396-2.44-2.446v-7.294h4.73V7.874H15.6V1.592h-3.714s-.167.053-.182.186c-.218 1.935-1.144 5.33-4.988 6.688v3.637h2.927v7.677c0 2.8 1.7 6.7 7.3 6.6 1.863-.03 3.934-.795 4.392-1.453l-1.22-3.54c-.52.213-1.415.413-2.115.455z"
                />
              </svg>
            </span>
            <span class="rrssb-text">tumblr</span>
          </a>
        </li>
      ';
      break;

      // reddit
    case 'reddit':
      return '
        <li class="rrssb-reddit">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M11.794 15.316c0-1.03-.835-1.895-1.866-1.895-1.03 0-1.893.866-1.893 1.896s.863 1.9 1.9 1.9c1.023-.016 1.865-.916 1.865-1.9zM18.1 13.422c-1.03 0-1.895.864-1.895 1.895 0 1 .9 1.9 1.9 1.865 1.03 0 1.87-.836 1.87-1.865-.006-1.017-.875-1.917-1.875-1.895zM17.527 19.79c-.678.68-1.826 1.007-3.514 1.007h-.03c-1.686 0-2.834-.328-3.51-1.005-.264-.265-.693-.265-.958 0-.264.265-.264.7 0 1 .943.9 2.4 1.4 4.5 1.402.005 0 0 0 0 0 .005 0 0 0 0 0 2.066 0 3.527-.46 4.47-1.402.265-.264.265-.693.002-.958-.267-.334-.688-.334-.988-.043z"
                />
                <path d="M27.707 13.267c0-1.785-1.453-3.237-3.236-3.237-.792 0-1.517.287-2.08.76-2.04-1.294-4.647-2.068-7.44-2.218l1.484-4.69 4.062.955c.07 1.4 1.3 2.6 2.7 2.555 1.488 0 2.695-1.208 2.695-2.695C25.88 3.2 24.7 2 23.2 2c-1.06 0-1.98.616-2.42 1.508l-4.633-1.09c-.344-.082-.693.117-.803.454l-1.793 5.7C10.55 8.6 7.7 9.4 5.6 10.75c-.594-.45-1.3-.75-2.1-.72-1.785 0-3.237 1.45-3.237 3.2 0 1.1.6 2.1 1.4 2.69-.04.27-.06.55-.06.83 0 2.3 1.3 4.4 3.7 5.9 2.298 1.5 5.3 2.3 8.6 2.325 3.227 0 6.27-.825 8.57-2.325 2.387-1.56 3.7-3.66 3.7-5.917 0-.26-.016-.514-.05-.768.965-.465 1.577-1.565 1.577-2.698zm-4.52-9.912c.74 0 1.3.6 1.3 1.3 0 .738-.6 1.34-1.34 1.34s-1.343-.602-1.343-1.34c.04-.655.596-1.255 1.396-1.3zM1.646 13.3c0-1.038.845-1.882 1.883-1.882.31 0 .6.1.9.21-1.05.867-1.813 1.86-2.26 2.9-.338-.328-.57-.728-.57-1.26zm20.126 8.27c-2.082 1.357-4.863 2.105-7.83 2.105-2.968 0-5.748-.748-7.83-2.105-1.99-1.3-3.087-3-3.087-4.782 0-1.784 1.097-3.484 3.088-4.784 2.08-1.358 4.86-2.106 7.828-2.106 2.967 0 5.7.7 7.8 2.106 1.99 1.3 3.1 3 3.1 4.784C24.86 18.6 23.8 20.3 21.8 21.57zm4.014-6.97c-.432-1.084-1.19-2.095-2.244-2.977.273-.156.59-.245.928-.245 1.036 0 1.9.8 1.9 1.9-.016.522-.27 1.022-.57 1.327z"
                />
              </svg>
            </span>
            <span class="rrssb-text">reddit</span>
          </a>
        </li>
      ';
      break;

      // hackernews
    case 'hackernews':
      return '
        <li class="rrssb-hackernews">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path fill="#FFF" d="M14 13.626l-4.508-9.19H6.588l6.165 12.208v6.92h2.51v-6.92l6.15-12.21H18.69" />
              </svg>
            </span>
            <span class="rrssb-text">hackernews</span>
          </a>
        </li>
      ';
      break;

      // youtube
    case 'youtube':
      return '
        <li class="rrssb-youtube">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M27.688 8.512c0-2.268-1.825-4.093-4.106-4.093H4.39C2.143 4.42.312 6.243.312 8.51v10.976c0 2.268 1.825 4.093 4.076 4.093h19.19c2.275 0 4.107-1.824 4.107-4.092V8.512zm-16.425 10.12V8.322l7.817 5.154-7.817 5.156z" />
              </svg>
            </span>
            <span class="rrssb-text">youtube</span>
          </a>
        </li>
      ';
      break;

      // pocket
    case 'pocket':
      return '
        <li class="rrssb-pocket">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg width="32" height="28" viewBox="0 0 32 28" xmlns="http://www.w3.org/2000/svg">
                <path d="M28.782.002c2.03.002 3.193 1.12 3.182 3.106-.022 3.57.17 7.16-.158 10.7-1.09 11.773-14.588 18.092-24.6 11.573C2.72 22.458.197 18.313.057 12.937c-.09-3.36-.05-6.72-.026-10.08C.04 1.113 1.212.016 3.02.008 7.347-.006 11.678.004 16.006.002c4.258 0 8.518-.004 12.776 0zM8.65 7.856c-1.262.135-1.99.57-2.357 1.476-.392.965-.115 1.81.606 2.496 2.453 2.334 4.91 4.664 7.398 6.966 1.086 1.003 2.237.99 3.314-.013 2.407-2.23 4.795-4.482 7.17-6.747 1.203-1.148 1.32-2.468.365-3.426-1.01-1.014-2.302-.933-3.558.245-1.596 1.497-3.222 2.965-4.75 4.526-.706.715-1.12.627-1.783-.034-1.597-1.596-3.25-3.138-4.93-4.644-.47-.42-1.123-.647-1.478-.844z"
                />
              </svg>
            </span>
            <span class="rrssb-text">pocket</span>
          </a>
        </li>
      ';
      break;

      // github
    case 'github':
      return '
        <li class="rrssb-github">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">
                <path d="M13.97 1.57c-7.03 0-12.733 5.703-12.733 12.74 0 5.622 3.636 10.393 8.717 12.084.637.13.87-.277.87-.615 0-.302-.013-1.103-.02-2.165-3.54.77-4.29-1.707-4.29-1.707-.578-1.473-1.413-1.863-1.413-1.863-1.154-.79.09-.775.09-.775 1.276.104 1.96 1.316 1.96 1.312 1.135 1.936 2.99 1.393 3.712 1.06.116-.823.445-1.384.81-1.704-2.83-.32-5.802-1.414-5.802-6.293 0-1.39.496-2.527 1.312-3.418-.132-.322-.57-1.617.123-3.37 0 0 1.07-.343 3.508 1.305 1.016-.282 2.105-.424 3.188-.43 1.082 0 2.167.156 3.198.44 2.43-1.65 3.498-1.307 3.498-1.307.695 1.754.258 3.043.13 3.37.815.903 1.314 2.038 1.314 3.43 0 4.893-2.978 5.97-5.814 6.286.458.388.876 1.16.876 2.358 0 1.703-.016 3.076-.016 3.482 0 .334.232.748.877.61 5.056-1.687 8.7-6.456 8.7-12.08-.055-7.058-5.75-12.757-12.792-12.75z"
                />
              </svg>
            </span>
            <span class="rrssb-text">github</span>
          </a>
        </li>
      ';
      break;

      // vk
    case 'vk':
      return '
        <li class="rrssb-vk">
          <a href="' . $url . '" class="popup">
            <span class="rrssb-icon">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="70 70 378.7 378.7"><path d="M254.998 363.106h21.217s6.408-.706 9.684-4.23c3.01-3.24 2.914-9.32 2.914-9.32s-.415-28.47 12.796-32.663c13.03-4.133 29.755 27.515 47.482 39.685 13.407 9.206 23.594 7.19 23.594 7.19l47.407-.662s24.797-1.53 13.038-21.027c-.96-1.594-6.85-14.424-35.247-40.784-29.728-27.59-25.743-23.126 10.063-70.85 21.807-29.063 30.523-46.806 27.8-54.405-2.596-7.24-18.636-5.326-18.636-5.326l-53.375.33s-3.96-.54-6.892 1.216c-2.87 1.716-4.71 5.726-4.71 5.726s-8.452 22.49-19.714 41.618c-23.77 40.357-33.274 42.494-37.16 39.984-9.037-5.842-6.78-23.462-6.78-35.983 0-39.112 5.934-55.42-11.55-59.64-5.802-1.4-10.076-2.327-24.915-2.48-19.046-.192-35.162.06-44.29 4.53-6.072 2.975-10.757 9.6-7.902 9.98 3.528.47 11.516 2.158 15.75 7.92 5.472 7.444 5.28 24.154 5.28 24.154s3.145 46.04-7.34 51.758c-7.193 3.922-17.063-4.085-38.253-40.7-10.855-18.755-19.054-39.49-19.054-39.49s-1.578-3.873-4.398-5.947c-3.42-2.51-8.2-3.307-8.2-3.307l-50.722.33s-7.612.213-10.41 3.525c-2.488 2.947-.198 9.036-.198 9.036s39.707 92.902 84.672 139.72c41.234 42.93 88.048 40.112 88.048 40.112"/></svg>
            </span>
            <span class="rrssb-text">vk.com</span>
          </a>
        </li>
      ';
      break;
  }
}
