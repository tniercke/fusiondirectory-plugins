<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2014-2016 FusionDirectory
 
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

class ejbcaManagement extends simpleManagement
{
  protected $objectTypes      = array('ejbcaCertificate');
  protected $autoActions      = FALSE;
  protected $skipSnapHandler  = TRUE;

  protected $autoFilterAttributes = array('dn', 'cn', 'description', 'userCertificate');

  public static function plInfo()
  {
    return array(
      'plShortName'   => _('EJBCA'),
      'plDescription' => _('EJBCA certificates management'),
      'plIcon'        => 'geticon.php?context=applications&icon=ejbca&size=48',
      'plSection'     => 'accounts',
      'plPriority'    => 50,
      'plManages'     => array('ejbcaCertificate'),

      'plProvidedAcls' => array()
    );
  }

  function configureHeadpage ()
  {
    parent::configureHeadpage();
    unset($this->headpage->xmlData['actionmenu']['action'][0]);
    $downloadAction =
      array(
        'name'  => 'download',
        'type'  => 'entry',
        'image' => 'geticon.php?context=actions&amp;icon=download&amp;size=16',
        'label' => _('Download'),
        'acl'   => 'ejbcaCertificate/ejbcaCertificate[r]',
      );
    array_unshift($this->headpage->xmlData['actiontriggers']['action'], $downloadAction);
    $this->registerAction('download', 'downloadEntry');
  }

  function downloadEntry($action, $targets, $all)
  {
    $entry = $this->headpage->getEntry($targets[0]);
    session::set('binary', $entry['userCertificate;binary'][0]);
    session::set('binarytype', 'octet-stream');
    session::set('binaryfile', $entry['cn'][0].'.der');
    header('location: getbin.php');
    exit();
  }
}
?>
