#!/usr/bin/perl

foreach(split(/&/, shift)){print qq{s_string("&$_");\n}}
