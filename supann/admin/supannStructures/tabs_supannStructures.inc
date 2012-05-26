<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2012  FusionDirectory

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
  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

class supannStructurestabs extends tabs
{

  function supannStructurestabs($config, $data, $dn, $category,
                                $hide_refs = FALSE, $hide_acls = FALSE)
  {
    tabs::tabs($config, $data, $dn, $category, $hide_refs, $hide_acls);

    /* Detect the base class  (The classs which extends from department) */
    foreach ($this->by_object as $name => $object) {
      $this->base_name = get_class($object);
      break;
    }

    /* Add references/acls/snapshots */
    $this->addSpecialTabs();
    if (isset($this->by_object['acl'])) {
      $this->by_object['acl']->skipTagging = TRUE;
    }
  }

  function save_object($save_current = FALSE)
  {
    tabs::save_object($save_current);
  }

  function delete()
  {
    tabs::delete();
  }

  function save($ignore_account = FALSE)
  {
    $baseobject = $this->by_object[$this->base_name];

    /* Check for new 'dn', in order to propagate the
       'dn' to all plugins */
    $this->dn = $baseobject->compute_dn();

    return tabs::save();
  }
}

?>