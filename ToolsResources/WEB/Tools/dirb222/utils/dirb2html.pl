#!/usr/bin/perl
#
# Dirb2HTML , lee la salida de DIRB de un fichero y produce la version HTML
# WZZ 2010 
#
# URI::Find build depends on Module::Build Test::More and needs URI.pm URI::URL to run.

use CGI qw(escapeHTML);
use URI::Find;

if(!$ARGV[0]) { print "Usage: ./dirb2html.pl dirb_output_file\n"; exit; }

$infile=$ARGV[0];

my $finder = URI::Find->new(sub {
      my($uri, $orig_uri) = @_;
      return qq|<a href="$uri" target="_blank">$orig_uri</a>|;
});

open(FILE,$infile);

print "<HTML>\n";
print "<HEAD><TITLE>".$infile." - Dirb HTML</TITLE></HEAD>\n";
print "<BODY BGCOLOR=\"#FFFFFF\" TEXT=\"#000000\">\n"; 
print "<PRE>\n";

while ( $l=<FILE> )
{
 chop($l);
 $text = $l;
 $finder->find(\$text, \&escapeHTML);
 print "$text\n";
}

print "</PRE>\n";
print "</BODY>\n";
print "</HTML>\n";

