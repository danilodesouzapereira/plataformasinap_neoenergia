  SELECT
    ArjTipo.intArranjoId          AS ArranjoId,
    Arranjo.intTipoArranjoId      AS TipoArranjoId,
    Arranjo.strDescricao          AS Descricao,
    Arranjo.strExtId              AS ExtId,
    Arranjo.bolTipico             AS Tipico,
    Arranjo.fltResTerra           AS ResTerra,
    Arranjo.fltVmin               AS Vmin,
    Arranjo.fltVmax               AS Vmax,
    Arranjo.intColorId            AS ColorId,
    Arranjo.intStatus             AS Status,
    Arranjo.intTipoFaseId         AS ArranjoTipoFaseId,
    Arranjo.dttVersao             AS DataVersao,
    ArjTipo.blbMatZ               AS MatZ,
    ArjTipo.blbMatC               AS MatC,
    ArjTipo.fltIadm               AS Iadm
  FROM
    Arranjo_03 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = -1) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = [p_ArranjoId] )
    );
