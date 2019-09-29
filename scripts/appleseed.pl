#!/usr/bin/perl
use v5.14;
use strict;
use warnings;
use utf8;

use File::Copy;
use File::Path;

sub QTPATH () { '/usr/local/opt/qt' }
sub MACDEPLOYQT() { QTPATH.'/bin/macdeployqt' }

die "usage: $0 appname bin qmldir" unless $@ARGV != 3;
my ($appname, $bin, $qmldir) = @ARGV;
my $binpath = sprintf("./%s.app/Contents/MacOs", $appname);
mkpath($binpath);
copy($bin, $binpath);

say "running macdeployqt.."
system(
    MACDEPLOYQT,
    sprintf("./%s.app", $appname),
    sprintf("-qmldir=%s", $qmldir),
    sprintf("-qmlimport=%s", QTPATH.'/qml')
);
say "OK!"

__END__

=encoding utf8

=head1 NAME

appleseed.pl - Mach-O 형식의 Qt 실행 파일을 macOS 애플리케이션으로 최소한의 부트스트래핑을 해 줍니다. 
(라고 거창하게 써 놨으나 실제 하는건 별로 없음)

=head1 SYNOPSIS

    #                                               v binary
    $ perl $AZALEA_HOME/scripts/appleseed.pl Azalea azalea
    #                                        ^ Application name
    
