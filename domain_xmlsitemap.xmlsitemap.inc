<?php
// $Id$

/**
 * @file
 * XML sitemap functions for the domain_xmlsitemap.module.
 */

/**
 * Implements hook_xmlsitemap_context_info().
 */
function domain_xmlsitemap_xmlsitemap_context_info() {
  $context['domain'] = array(
    'label' => t('Domain'),
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
 * Implements hook_xmlsitemap_context_fallback().
 */
function domain_xmlsitemap_xmlsitemap_context_fallback() {
  $context['domain'] = 0;
  return $context;
}

/**
 * Implements hook_xmlsitemap_context_url_options().
 */
function domain_xmlsitemap_xmlsitemap_context_url_options(array $context) {
  $options = array();
  if (isset($context['domain']) && $domain = domain_load($context['domain'])) {
    $options['base_url'] = rtrim($domain['path'], '/');
  }
  return $options;
}
