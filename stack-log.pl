#!/usr/bin/env perl
use warnings;
use strict;

my %depth = ();
my $executable = "~/mono/mono/mini/mono-sgen";
my %index = ();
my $sampling_rate = 50;
my %min = ();
my %max = ();
my %data = ();
while (<>) {
  my ($timestamp, $thread, $type, $from, $to) = split;
  $depth{$thread} += ($type eq 'enter' ? 1 : $type eq 'exit' ? -1 : 0);
  if (!defined($min{$thread}) || $depth{$thread} < $min{$thread}) {
    $min{$thread} = $depth{$thread};
  }
  if (!defined($max{$thread}) || $depth{$thread} > $max{$thread}) {
    $max{$thread} = $depth{$thread};
  }
  if ($index{$thread}++ % $sampling_rate == 0) {
    $data{$thread} = [] unless exists $data{$thread};
    push @{$data{$thread}}, "$timestamp, $min{$thread}, $max{$thread}";
    undef $min{$thread};
    undef $max{$thread};
  }
}

sub zip {
  my @result = ();
  push @result, [map { shift @$_ } @_] while grep { grep {  defined $_ } @$_ } @_;
  \@result
}

my $results = zip(values %data);
for my $result (@$results) {
  print((join ', ', map { $_ // ',,' } @$result), "\n");
}
