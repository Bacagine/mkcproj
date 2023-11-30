##
# _mkcproj_complete.sh
#
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
#  
# Description: Autocomplete script of mkcproj, the generator of new C projects
# 
# Copyright (C) 2023 Gustavo Bacagine
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Date: 04/10/2023
# 

_mkcproj_complete()
{
  local cur_word
  local prev_word
  local long_type_list
  local short_type_list

  cur_word="${COMP_WORDS[COMP_CWORD]}"
  prev_word="${COMP_WORDS[COMP_CWORD-1]}"

  long_type_list=`mkcproj --help --version --trace \
    --debug-level --colored-log --conf-filename`
  short_type_list=`mkcproj -h -v -t -l -c -C`
  
  case "$cur_word" in
    --*) COMPREPLY=( $(compgen -W "${long_type_list}" -- ${cur_word}) );;
    -*) COMPREPLY=( $(compgen -W "${short_type_list}" -- ${cur_word}) );;
  esac

  return 0
}

complete -F _mkcproj_complete -o default mkcproj

