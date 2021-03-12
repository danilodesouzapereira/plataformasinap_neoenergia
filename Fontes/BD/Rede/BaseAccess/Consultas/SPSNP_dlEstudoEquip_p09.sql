  DELETE FROM
    EquipamentoBarra
  WHERE
    intEstudoId = [p_EstudoId] AND
    intEquipId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN EquipamentoBarra EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intEquipId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1))
    );

