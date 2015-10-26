<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2015  FusionDirectory

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

class DnsRecordAttribute extends CompositeAttribute
{
  public static $types = array(
    'aRecord'     => 'A',
    'a6Record'    => 'A6',
    'aAAARecord'  => 'AAAA',
    'aFSDBRecord' => 'AFSDB',
    'certRecord'  => 'CERT',
    'cNAMERecord' => 'CNAME',
    'dSRecord'    => 'DS',
    'hInfoRecord' => 'HINFO',
    'KeyRecord'   => 'KEY',
    'kXRecord'    => 'KX',
    'LocRecord'   => 'LOC',
    'mDRecord'    => 'MD',
    'mInfoRecord' => 'MINFO',
    'mXRecord'    => 'MX',
    'nAPTRRecord' => 'NAPTR',
    'nSRecord'    => 'NS',
    'nSECRecord'  => 'NSEC',
    'nXTRecord'   => 'NXT',
    'pTRRecord'   => 'PTR',
    'rRSIGRecord' => 'RRSIG',
    'SigRecord'   => 'SIG',
    'sRVRecord'   => 'SRV',
    'sSHFPRecord' => 'SSHFP',
    'tXTRecord'   => 'TXT',
  );

  function __construct($label, $description, $ldapName, $required, $acl = '')
  {
    $attributes = array(
      new SelectAttribute(_('Type'), '', $ldapName.'_type', $required, array_keys(self::$types), '', array_values(self::$types)),
      new HiddenAttribute($ldapName.'_nofield')
    );
    parent::__construct($description, $ldapName, $attributes, FALSE, FALSE, $acl, $label);
    $this->setLinearRendering(TRUE);
    $this->attributes[0]->setSubmitForm(TRUE);
    $this->updateFields();
    $this->setRequired($required);
  }

  protected function updateFields()
  {
    $type = $this->attributes[0]->getValue();
    $ldapName = $this->getLdapName().'_'.$type.'_content';
    if ($this->attributes[1]->getLdapName() != $ldapName) {
      switch ($type) {
        case 'aRecord':
          $this->attributes[1] = new IPv4Attribute(_('Target'), '', $ldapName, TRUE);
        break;
        case 'aAAARecord':
          $this->attributes[1] = new IPv6Attribute(_('Target'), '', $ldapName, TRUE);
        break;
        case 'mXRecord':
          $this->attributes[1] = new CharSeparatedCompositeAttribute(
            _('MX Record'),
            $ldapName,
            array(
              new IntAttribute(
                _('Priority'), _('Preference given to this RR among others at the same owner, lower values are preferred'),
                $ldapName.'_priority', TRUE,
                0, FALSE
              ),
              new StringAttribute(
                _('Target'), _('Domain name which specifies a host willing to act as a mail exchange for the owner name'),
                $ldapName.'_target', TRUE
              ),
            ),
            ' '
          );
        break;
        case 'nSRecord':
          $this->attributes[1] = new StringAttribute(
            _('Target'), _('Domain name which specifies a host which should be authoritative for the specified class and domain'),
            $ldapName, TRUE
          );
        break;
        case 'sRVRecord':
          $this->attributes[1] = new CharSeparatedCompositeAttribute(
            _('SRV Record'),
            $ldapName,
            array(
              new IntAttribute(
                _('Priority'), _('Priority of the target host, lower value means more preferred'),
                $ldapName.'_priority', TRUE,
                0, FALSE
              ),
              new IntAttribute(
                _('Weight'), _('Relative weight for records with the same priority, higher value means more preferred'),
                $ldapName.'_weight', TRUE,
                0, FALSE
              ),
              new IntAttribute(
                _('Port'), _('TCP or UDP port on which the service is to be found'),
                $ldapName.'_port', TRUE,
                0, FALSE
              ),
              new StringAttribute(
                _('Target'), _('Canonical hostname of the machine providing the service, ending in a dot'),
                $ldapName.'_target', TRUE
              ),
            ),
            ' '
          );
        break;
        default:
          $this->attributes[1] = new StringAttribute(_('Content'), _('Content of this record'), $ldapName, TRUE);
        break;
      }
    }
    $this->setAttributes($this->attributes); // Update ACLs
  }

  function applyPostValue()
  {
    parent::applyPostValue();
    $this->updateFields();
  }

  function setValue ($values)
  {
    if (!is_array($values)) {
      $values = $this->inputValue($values);
    }
    $this->attributes[0]->setValue($values[0]);
    $this->updateFields();
    $this->attributes[1]->setValue($values[1]);
  }

  function resetToDefault ()
  {
    $this->attributes[0]->resetToDefault();
    $this->updateFields();
    $this->attributes[1]->resetToDefault();
  }
}