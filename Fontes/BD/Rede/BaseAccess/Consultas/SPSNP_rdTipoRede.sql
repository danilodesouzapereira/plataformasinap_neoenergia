  SELECT
    TipoRede.intTipoRedeId,
    TipoRede.strCodigo,
    TipoRede.strDescricao,
    TipoRede.fltVMin,
    TipoRede.fltVMax,
    TipoRede.intColorId,
    TipoRede.intStatus,
    (
    SELECT
      COUNT(Rede.intTipoRedeId)
    FROM
      Rede
    WHERE
      (
      ( [p_EstudoId] = -1) OR
      (( [p_EstudoId] <> -1) AND (Rede.intEstudoId = [p_EstudoId] ))
      ) AND
      Rede.intTipoRedeId = TipoRede.intTipoRedeId
    ) AS intQuantidade
  FROM
    TipoRede
  WHERE
    (
      ( [p_EstudoId] = -1) OR
      (
        ( [p_EstudoId] <> -1) AND
        (
        TipoRede.intTipoRedeId IN
          (
          SELECT DISTINCT
            Rede.intTipoRedeId
          FROM
            Rede
          WHERE
            Rede.intEstudoId = [p_EstudoId]
          )
        )
      )
    );
