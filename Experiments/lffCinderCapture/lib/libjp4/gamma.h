/*
  Copyright 2010 Paulo Henrique Silva <ph.silva@gmail.com>

  This file is part of jp4-tools.

  jp4-tools is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  jp4-tools is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with jp4-tools.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMA_H
#define GAMMA_H 1

class Gamma {
 public:

  static void reverseTable(double gamma, double gamma_scale, double black, unsigned short* rgTable);

};

#endif // GAMMA_H
