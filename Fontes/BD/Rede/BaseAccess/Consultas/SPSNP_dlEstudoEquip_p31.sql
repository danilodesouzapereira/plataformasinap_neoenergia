  DELETE FROM
    Trecho
  WHERE
    intEstudoId = [p_EstudoId] AND
    intTrechoId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Trecho EqpAux INNER JOIN Equipamento ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        EqpAux.intTrechoId  = Equipamento.intEquipId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 28
    );