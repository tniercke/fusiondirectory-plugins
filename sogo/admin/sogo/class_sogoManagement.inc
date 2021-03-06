<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2013-2016 FusionDirectory
 
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

class sogoManagement extends simpleManagement
{
  // Tab definition
  protected $objectTypes  = array('sogoResource');

  protected $departmentBrowser      = TRUE;
  protected $departmentRootVisible  = TRUE;
  protected $baseMode               = TRUE;

  /* Return plugin informations for acl handling  */
  public static function plInfo()
  {
    return array(
      'plShortName'   => _('SOGo'),
      'plDescription' => _('SOGo management'),
      'plIcon'        => 'geticon.php?context=applications&icon=sogo&size=48',
      'plSection'     => 'systems',
      'plPriority'    => 50,
      'plCategory'    => array('sogo' => array('description'  => _('SOGo'),
                                              'objectClass'  => array('sogoResource'))),
      'plManages'     => array('sogoResource'),

      'plProvidedAcls' => array()
    );
  }
}
?>
