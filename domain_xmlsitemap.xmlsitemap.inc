<?php
// $Id$

/**
 * @file
 * XML sitemap integration functions for domain_xmlsitemap.module.
 */

/**
 * Implements hook_xmlsitemap_context_info().
 */
function domain_xmlsitemap_xmlsitemap_context_info() {
  $context['domain'] = array(
    'label' => t('Domain'),
    'default' => 0,
  );
  return $context;
}

/**
 * Implements hook_xmlsitemap_context().
 */
function domain_xmlsitemap_xmlsitemap_context() {
  $domain = domain_get_domain();
  $context['domain'] = $domain['domain_id'];
  return $context;
}

/**
 * Implements hook_xmlsitemap_sitemap_uri_alter().
 */
function domain_xmlsitemap_xmlsitemap_sitemap_uri_alter(&$path, array &$options, stdClass $sitemap) {
  if (isset($sitemap->context['domain']) && $domain = domain_load($sitemap->context['domain'])) {
    $options['base_url'] = rtrim($domain['path'], '/');
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function domain_xmlsitemap_form_xmlsitemap_sitemap_edit_form_alter(&$form, $form_state) {
  $domains = domain_domains();
  $options = array();
  foreach ($domains as $domain) {
    $options[$domain['domain_id']] = $domain['path'];
  }
  $form['context']['domain'] = array(
    '#type' => 'select',
    '#title' => t('Domain'),
    '#options' => $options,
    '#default_value' => isset($form['#sitemap']->context['domain']) ? $form['#sitemap']->context['domain'] : '',
    '#access' => !empty($options),
  );
}

/**
 * Implements hook_query_TAG_alter().
 */
function domain_xmlsitemap_query_xmlsitemap_generate_alter(QueryAlterableInterface $query) {
  $sitemap = $query->getMetaData('sitemap');
  if (!isset($sitemap->context['domain'])) {
    return;
  }

  // @todo Join here may cause duplicate records. Investigate using a subquery.
  $domain_alias = $query->leftJoin('domain_access', 'da', "x.type = 'node' AND x.id = da.nid");
  $domain_condition = db_or();
  $domain_condition->isNull('da.gid');
  $domain_condition->condition(db_and()
    ->condition("{$domain_alias}.gid", 0)
    ->condition("{$domain_alias}.realm", 'domain_site')
  );
  $domain_condition->condition(db_and()
    ->condition("{$domain_alias}.gid", $sitemap->context['domain'])
    ->condition("{$domain_alias}.realm", 'domain_id')
  );
  $query->condition($domain_condition);
}
