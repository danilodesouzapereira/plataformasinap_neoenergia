  SELECT
    Agrupamento.intAgrupamentoId   AS AgrupamentoId,
    Agrupamento.strCodigo          AS Codigo
  FROM
    Agrupamento
  WHERE
    intEstudoId = [p_estudoId] AND
    (( [p_AgrupamentoId] = -1) OR (Agrupamento.intAgrupamentoId = [p_AgrupamentoId] ));
