#!/usr/bin/perl

#version: 1.0
#author: Dave Aitel
#license: GPL v 2.0

#makes a spike.c from a http capture

sub parsefirstline {

#first split up the line with a request
    my ($request, $url, $httpver) = split(/ /, shift);
    my ($base, $variables) = split (/\?/, $url);    
    print "s_string(\"$request $base?\");\n";
#eh? What does this next line do again.
    print "s_setfirstvariable();\n";
    print "s_string_variables('&',\"$variables\");\n";

#remove \r\n - we do this to make things look nicer
    chop $httpver;    chop $httpver; 
#    $httpver = s/\\r//;
#    $httpver = s/\\n//;

    print "s_string(\" $httpver\\r\\n\");\n"; #add \r\n
    
}

#open every file given on argument list for processing

print "/* Start webfuzzprelude.c */\n\n";
#apparantly this causes problems
#system "cat webfuzzprelude.c";
print `cat webfuzzprelude.c`;
print "/*end webfuzzprelude.c */\n\n";

open IN, shift;

my $firstline = <IN>;

#print "Firstline = $firstline";
#first parse the first line, which is GET or POST then URL+ARGS then VERSION
parsefirstline ($firstline);

while(<IN>) {
#    print "HEADER $_";
    if ( $_ eq "\r\n" ) {
#we're done with the headers

	last;
    }

    if ($_ =~ /^Cookie: (.+)$/) {
#get cookie
	my $cookie = $1;
#remove pesky \r\n from cookie
	chop $cookie;
	chop $cookie;
	chomp $cookie;

	#print "cookie is $cookie\n";
	print "s_string_or_env(\"COOKIE\",\"Cookie: $cookie\");\n";
	print "s_string(\"\\r\\n\");\n";
	next;
    }

#Also need to trap Content-Length: here
    if ($_ =~ /^Content-[Ll]ength: (.+)$/) {
	print "s_string(\"Content-Length: \");\n";
	print "s_blocksize_string(\"post\",7);\n";
	print "s_string(\"\\r\\n\");\n";
	next;
    }


    my $line = $_;
#remove /r/n
    chop $line;
    chop $line;
    print "s_string(\"$line\\r\\n\");\n";
    
}

#skip the empty line
#<IN>;

print "/*Done with Headers*/\n";
print "s_string(\"\\r\\n\");\n";

print "s_block_start(\"post\");\n";
print "s_setfirstvariable();\n";

while (<IN>) {
#    print "Content as follows $_\n";
    print "s_string_variables('&',\"$_\");\n";
}

print "s_block_end(\"post\");\n";

print "/* Start webfuzzpostlude.c */ \n\n";
print `cat webfuzzpostlude.c`;
print "/* End webfuzzpostlude.c */ \n\n";











