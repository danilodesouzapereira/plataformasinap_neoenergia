  SELECT
    ArjTipo.intArranjoId                                            AS ArranjoId,
    Arranjo.intTipoArranjoId                                        AS TipoArranjoId,
    Arranjo.strDescricao                                            AS Descricao,
    Arranjo.strExtId                                                AS ExtId,
    Arranjo.bolTipico                                               AS Tipico,
    Arranjo.fltResTerra                                             AS ResTerra,
    Arranjo.fltVmin                                                 AS Vmin,
    Arranjo.fltVmax                                                 AS Vmax,
    Arranjo.intColorId                                              AS ColorId,
    Arranjo.intStatus                                               AS Status,
    Arranjo.intNumCabos                                             AS NumCabos,
    Arranjo.intTipoFaseId                                           AS ArranjoTipoFaseId,
    Arranjo.dttVersao                                               AS DataVersao,
    ArjTipo.intCaboId                                               AS CaboId,
    ArjTipo.intCaboItem                                             AS CaboItem,
    IIF(ISNULL(ArjTipo.intTipoFaseId),  -1, ArjTipo.intTipoFaseId)  AS TipoFaseId,
    IIF(ISNULL(ArjTipo.intSuporteId),   -1, ArjTipo.intSuporteId)   AS SuporteId,
    IIF(ISNULL(ArjTipo.intFixacaoItem), -1, ArjTipo.intFixacaoItem) AS FixacaoItem
  FROM
    Arranjo_00_01 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = -1) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = [p_ArranjoId] )
    )
  ORDER BY
    ArjTipo.intArranjoId,
    ArjTipo.intTipoFaseId;
