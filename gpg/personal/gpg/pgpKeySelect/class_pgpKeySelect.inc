<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2013  FusionDirectory

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

class pgpKeySelect extends management
{
  protected $skipFooter = TRUE;
  protected $skipHeader = TRUE;

  function __construct($config, $ui)
  {
    $this->config = $config;
    $this->ui     = $ui;

    $ldap = $this->config->get_ldap_link();
    $ldap->cat('cn=pgpServerInfo,'.$this->config->current['BASE']);
    if ($pgpServerInfo = $ldap->fetch()) {
      $this->storagePoints = array(
        preg_replace(
          '/'.$this->config->current['BASE'].'$/', '',
          $pgpServerInfo['pgpBaseKeySpaceDN'][0]
        )
      );
    } else {
      msg_dialog::display(_('Configuration error'), _('You need to configure GPG base dn through the addons section first'));
    }

    $filter = new filter(get_template_path('pgpKeySelect-filter.xml', TRUE, dirname(__FILE__)));
    $filter->setObjectStorage($this->storagePoints);
    $this->setFilter($filter);

    // Build headpage
    $headpage = new listing(get_template_path('pgpKeySelect-list.xml', TRUE, dirname(__FILE__)));
    $headpage->setBase($this->config->current['BASE']);
    $headpage->setFilter($filter);

    parent::__construct($config, $ui, '', $headpage);
  }
}

class PgpKeySelectDialog extends GenericSelectDialog
{
  protected $dialogClass = 'pgpKeySelect';
}

class PgpKeyAttribute extends GenericDialogAttribute
{
  protected $dialogClass      = 'PgpKeySelectDialog';
  protected $height           = 90;
  protected $displayed_values = array('pgpKeyID','pgpUserID','pgpKeyCreateTime','pgpKeyType','pgpKeySize');

  function __construct ($label, $description, $ldapName, $required = FALSE, $defaultValue = array(), $acl = '')
  {
    parent::__construct ($label, $description, $ldapName, $required, $defaultValue, 'dn', NULL, $acl);
  }

  function addValue ($dn, $entry)
  {
    if (!in_array($dn, $this->value)) {
      $this->value[]    = $dn;
      $this->displays[] = $entry;
    }
  }

  protected function fillDisplayValue ($i)
  {
    $ldap = $this->plugin->config->get_ldap_link();

    $value = $this->value[$i];
    $ldap->cat($value);
    if (!$ldap->count()) {
      unset($this->value[$i]);
    } else {
      $attrs = $ldap->fetch();
      $this->displays[$i] = $attrs;
    }
  }

  function renderOnlyFormInput ()
  {
    $id   = $this->getHtmlId();
    $div  = new divSelectBox($id);
    $div->SetHeight($this->height);
    foreach ($this->value as $key => $value) {
      $fields = array();
      foreach ($this->displayed_values as $field) {
        if (isset($this->displays[$key][$field][0]) && !empty($this->displays[$key][$field][0])) {
          $display = $this->displays[$key][$field][0];
          if ($field == 'pgpKeySize') {
            $display = intval($display); // Remove useless 0 in front of the number
          } elseif ($field == 'pgpKeyCreateTime') {
            $display = DateTime::createFromFormat('Ymd', substr($display, 0, 8))->format('Y-m-d');
          }
          $fields[] = array('string' => $display);
        } else {
          $fields[] = array('string' => '&nbsp;');
        }
      }
      $img = '<input type="image" src="images/lists/trash.png" name="'.$id.'_del_'.$key.'" class="center"/>&nbsp;';
      $fields[] = array('html' => $img, 'attach' => 'style="border-right:0px;width:20px;"');
      $div->AddEntry($fields);
    }
    $smarty = get_smarty();
    $smarty->assign("div_$id", $div->DrawList());
    return '{$div_'.$id.'}'."\n";
  }
}

?>