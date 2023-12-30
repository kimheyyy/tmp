<?php

// ** MySQL settings - You can get this info from your web host ** //
define( 'DB_NAME', getenv('MARIA_DATABASE'));
define( 'DB_USER', getenv('MARIA_USER'));
define(	'DB_PASSWORD', getenv('MARIA_PASSWORD'));
define(	'DB_HOST', 'mariadb');
define( 'DB_CHARSET', 'utf8' );
define( 'DB_COLLATE', '' );

/**Authentication unique keys and salts.*/
define('AUTH_KEY',         'm[<>u^1@flPLh6M->q|s8_uaHFq9a~/8rB[?8=^H;mipnQqZqqKOGardKpvqsKaC');
define('SECURE_AUTH_KEY',  'r5,S}-]+6t+hIQ72u%<MmzsW4w|J!+MP=(65 b^)Vr%pUKmM-Kk3rwe;]q)9XiD1');
define('LOGGED_IN_KEY',    '+2W<Q2uDI6P??!u>UBN>Y+[Fw,YC]lvQx.kh9/vyTHrJebMj{]j8l*})#/S<e%9N');
define('NONCE_KEY',        'Oy_x!*k=OqfD1!z`m[0`>]k}=35IuJ%4yF6gnKt^]al8ygW&=iH`Z-Y;X1qVB.gb');
define('AUTH_SALT',        'KRH 6t v;}%3QH1H[;`.n|7uW<KRv]B9i(Ikn@TOCp1-9G#?-~fq`kr.~sqpW?E}');
define('SECURE_AUTH_SALT', '@uRKo[}d/S1T}^rr+_36e4-1mJ2Q+H6d]^db;_W-+?LyWeb+te`8>3sGP{>FhX@Q');
define('LOGGED_IN_SALT',   'Lc]z/7d,jH_<7|pm!l#vu-Qogf?6lE+hrR7nf<XW]p6 jCU,_$~:-eGykb dG/rN');
define('NONCE_SALT',       'w!6pt7In?M,vMvZIFA]kF[,(p.P/]98oqtS+g6uRl/&xP(XQiD3.%`-#IKm5OO)T');

$table_prefix = 'wp_';

define( 'WP_DEBUG', false );

/* That's all, stop editing! Happy publishing. */
/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', __DIR__ . '/' );
}

define('CONCATENATE_SCRIPTS', false);

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';