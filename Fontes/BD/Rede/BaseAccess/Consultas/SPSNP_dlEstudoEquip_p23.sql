  DELETE FROM
    Chave
  WHERE
    intEstudoId = [p_EstudoId] AND
    intChaveId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Chave EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId  = EqpAux.intChaveId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 9
    );