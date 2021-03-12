  SELECT
    ArjTipo.intArranjoId                                        AS ArranjoId,
    Arranjo.intTipoArranjoId                                    AS TipoArranjoId,
    Arranjo.strDescricao                                        AS Descricao,
    Arranjo.strExtId                                            AS ExtId,
    Arranjo.bolTipico                                           AS Tipico,
    Arranjo.fltResTerra                                         AS ResTerra,
    Arranjo.fltVmin                                             AS Vmin,
    Arranjo.fltVmax                                             AS Vmax,
    Arranjo.intColorId                                          AS ColorId,
    Arranjo.intStatus                                           AS Status,
    Arranjo.intTipoFaseId                                       AS ArranjoTipoFaseId,
    Arranjo.dttVersao                                           AS DataVersao,
    ArjTipo.fltZ0R                                              AS Z0R,
    ArjTipo.fltZ0X                                              AS Z0X,
    ArjTipo.fltZ1R                                              AS Z1R,
    ArjTipo.fltZ1X                                              AS Z1X,
    ArjTipo.fltC0                                               AS C0,
    ArjTipo.fltC1                                               AS C1,
    ArjTipo.fltIadm                                             AS Iadm,
    IIF(ISNULL(ArjTipo.intSuporteId), -1, ArjTipo.intSuporteId) AS SuporteId
  FROM
    Arranjo_02_04 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = -1) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = [p_ArranjoId] )
    );
