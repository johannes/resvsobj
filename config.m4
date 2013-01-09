PHP_ARG_ENABLE(resvsobj, whether to enable resvsobj support,
[  --enable-resvsobj           Enable resvsobj support])

if test "$PHP_RESVSOBJ" != "no"; then
  PHP_NEW_EXTENSION(resvsobj, resvsobj.c, $ext_shared)
fi
