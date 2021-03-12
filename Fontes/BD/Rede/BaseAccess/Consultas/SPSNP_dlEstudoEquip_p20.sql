  DELETE FROM
    Reator
  WHERE
    intEstudoId = [p_EstudoId] AND
    intReatorId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Reator EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intReatorId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 18
    );
