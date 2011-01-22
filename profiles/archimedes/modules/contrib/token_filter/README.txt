// $Id: README.txt,v 1.1.4.1 2009/05/04 09:06:52 pvhee Exp $

-- SUMMARY --

Token Filter is a very simple module to make token values available as an input filter.

-- REQUIREMENTS --

Token

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --

* Go to /admin/settings/filters and enable the token_filter for any of your existing filter type 
  or if you wish, create a new one.
* Then in the text where you use that input filter you can use substitution tokens with
  [token global site-name] etc. You can the global, user and any custom context available.

  Tokens typically available are:

  global:
  -------
  user-name
  user-id
  user-mail
  site-url
  site-name
  site-slogan
  site-mail
  site-date

  user:
  -----
  user
  user-raw
  uid
  mail
  reg-date
  reg-since
  log-date
  log-since
  date-in-tz
  account-url
  account-edit

-- CONTACT --

Current maintainers:
* Peter Vanhee (pvhee) - http://drupal.org/user/108811

Original Drupal 5 module written by:
* AsciiKewl

The Drupal 6 version of this project has been sponsored by:
* Youth Agora
  Innovating online youth information. 
  Visit http://www.youthagora.org for more information.