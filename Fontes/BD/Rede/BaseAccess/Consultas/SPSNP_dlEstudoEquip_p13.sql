  DELETE FROM
    CargaIP
  WHERE
    intEstudoId = [p_EstudoId] AND
    intCargaIPId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN CargaIP EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId  = EqpAux.intCargaIPId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 7
    );

