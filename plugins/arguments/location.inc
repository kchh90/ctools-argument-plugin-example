<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a Taxonomy term
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Location term: ID"),
  // keyword to use for %substitution
  'keyword' => 'term',
  'description' => t('Creates a single location taxonomy term from a store name.'),
  'context' => 'ctools_location_context',
);

/**
 * Returns a context object.
 */
function ctools_location_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('entity:taxonomy_term');
  }

  // Convert dashified argument back to spaces.
  $store_name = strtr($arg, '-', ' ');

  // EFQ to grab taxonomy term.
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'taxonomy_term')
    ->fieldCondition('field_store_name', 'value', $store_name, '=')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.

  $result = $query->execute();

  if (isset($result['taxonomy_term'])) {
    $tids = array_keys($result['taxonomy_term']);
    $location = entity_load('taxonomy_term', $tids);
  }
  // Get the first term object if multiple was found.
  $term = array_shift($location);

  // Create and return a context of taxonomy term
  $context = ctools_context_create('entity:taxonomy_term', $term);
  $context->original_argument = $arg;
  return $context;
}