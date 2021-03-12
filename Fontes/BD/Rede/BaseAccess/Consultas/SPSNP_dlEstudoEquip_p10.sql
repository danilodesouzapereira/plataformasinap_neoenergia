  DELETE FROM
    BarraRede
  WHERE
    intEstudoId = [p_EstudoId] AND
    intBarraId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN BarraRede EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intBarraId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 2
    );
