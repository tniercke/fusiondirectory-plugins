<?php

/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2011  FusionDirectory

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

class argonautMirrorConfig extends goService{

  var $cli_summary      = "This plugin is used within the ServerService Pluign \nand indicates that this server supports Argonaut service.";
  var $cli_description  = "Some longer text\nfor help";
  var $cli_parameters   = array("one" => "one is nice", "two" => "two is better");

  /* This plugin only writes its objectClass */
  var $objectclasses    = array("argonautMirrorConfig");
  var $attributes       = array("argonautMirrorDir",
                                "argonautCrawlerPackagesFolder",
                                "argonautLdap2repProxy",
                                "argonautLdap2repErrors",
                                "argonautLdap2repSource","argonautLdap2repGPGCheck",
                                "argonautLdap2repContents","argonautLdap2repVerbose");
  var $StatusFlag       = "";

  /* This class can't be assigned twice so it conflicts with itsself */
  var $conflicts        = array("argonautMirrorConfig");

  var $DisplayName      = "";
  var $dn               = NULL;
  var $acl;
  var $cn               = "";
  var $view_logged      = FALSE;

  var $argonautMirrorDir              = "/srv/www/debian";              // Where is you local debian mirror

  var $argonautCrawlerPackagesFolder  = "/var/cache/argonaut/packages"; // Where is stored the result produced by the crawler

  var $argonautLdap2repProxy          = "";
  var $argonautLdap2repErrors         = TRUE;
  var $argonautLdap2repSource         = FALSE;
  var $argonautLdap2repGPGCheck       = FALSE;
  var $argonautLdap2repContents       = FALSE;
  var $argonautLdap2repVerbose        = FALSE;

  var $ldapBooleans     = array("argonautLdap2repErrors",
                                "argonautLdap2repSource",   "argonautLdap2repGPGCheck",
                                "argonautLdap2repContents", "argonautLdap2repVerbose");

  function argonautMirrorConfig(&$config,$dn)
  {
    goService::goService($config,$dn);
    $this->DisplayName = _("Argonaut mirror settings");
    foreach ($this->ldapBooleans as $ldapBoolean) {
      $this->$ldapBoolean = ($this->$ldapBoolean == "TRUE"); // conversion from LDAP format to PHP boolean
    }
  }


  function execute()
  {
    $smarty = get_smarty();

    if ($this->is_account && !$this->view_logged) {
      $this->view_logged = TRUE;
      new log("view","server/".get_class($this),$this->dn);
    }

    $smarty->assign("argonautMirrorConfigACL", $this->getacl("argonautMirrorConfig")) ;
    $attributes = array();
    foreach ($this->attributes as $attribute) {
      $smarty->assign("$attribute" , $this->$attribute);
      $attributes[$attribute] = $this->$attribute;
    }
    $smarty->assign("attributes" , $attributes);
    $smarty->assign("crawlerIndice" , 1);
    $smarty->assign("ldap2repIndice" , 2);
    $smarty->assign("booleans" , $this->ldapBooleans);
    return $smarty->fetch(get_template_path("argonautMirrorConfig.tpl",TRUE,dirname(__FILE__)));
  }


  function getListEntry()
  {
    $fields               = goService::getListEntry();
    $fields['Message']    = _("Argonaut mirror settings");
    return $fields;
  }


  function save()
  {
    plugin::save();
    foreach ($this->ldapBooleans as $ldapBoolean) {
      $this->attrs[$ldapBoolean] = ($this->$ldapBoolean?"TRUE":"FALSE"); // conversion from PHP format to LDAP boolean
    }
    /* Check if this is a new entry ... add/modify */
    $ldap = $this->config->get_ldap_link();
    $ldap->cat($this->dn,array("objectClass"));
    if ($ldap->count()) {
      $ldap->cd($this->dn);
      $ldap->modify($this->attrs);
    } else {
      $ldap->cd($this->dn);
      $ldap->add($this->attrs);
    }
    if (!$ldap->success()) {
      msg_dialog::display(_("LDAP error"), msgPool::ldaperror($ldap->get_error(), $this->dn, 0, get_class()));
    }
    if ($this->initially_was_account) {
      $this->handle_post_events("modify");
      new log("modify","server/".get_class($this),$this->dn,array_keys($this->attrs),$ldap->get_error());
    } else {
      $this->handle_post_events("add");
      new log("create","server/".get_class($this),$this->dn,array_keys($this->attrs),$ldap->get_error());
    }
  }


  function PrepareForCopyPaste($source)
  {
    plugin::PrepareForCopyPaste($source);
  }

  function check()
  {
    $message = plugin::check();
    return $message;
  }

  function save_object()
  {
    if(isset($_POST['argonautMirrorConfigPosted'])) {
      plugin::save_object();
      if($this->acl_is_writeable("argonautMirrorConfig")) {
        foreach ($this->ldapBooleans as $ldapBoolean) {
          $this->$ldapBoolean = isset($_POST[$ldapBoolean]);
        }
        foreach ($this->attributes as $attribute) {
          if(in_array($attribute,$this->ldapBooleans)) {
            continue;
          } else if(isset($_POST[$attribute])) {
            $this->$attribute = $_POST[$attribute];
          }
        }
      }
    }
  }

  /* Return plugin informations for acl handling */
  static function plInfo()
  {
    return (array(
          "plShortName"   => _("Argonaut mirror settings"),
          "plDescription" => _("Argonaut mirror settings")." ("._("Services").")",
          "plSelfModify"  => FALSE,
          "plDepends"     => array(),
          "plPriority"    => 91,
          "plSection"     => array("administration"),
          "plCategory"    => array("server"),

          "plProvidedAcls"=> array(
            "argonautMirrorConfig"        =>_("Modify argonaut mirror settings"))
          ));
  }
}
// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>