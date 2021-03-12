  DELETE FROM
    TrafoZZ
  WHERE
    intEstudoId = [p_EstudoId] AND
    intTrafoZZId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN TrafoZZ EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intTrafoZZId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 26
    );
